/*
   Copyright (C) 2016 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file achievements.cc
 *
 * @author Kai Sterker
 * @brief Manages in-game achievements
 */

#include "achievements.h"
#include "game.h"
#include "gamedata.h"

py_callback* achievements::_callback = NULL;
vector<achievement_data> achievements::_achievements;

achievements::achievements()
{

}

achievements::~achievements()
{

}

bool achievements::create(const u_int8 & achievement, const u_int32 & bitmask)
{
	if (achievement == 0 || achievement == 255)
		return false;

	for (vector<achievement_data>::iterator i = _achievements.begin(); i != _achievements.end(); i++)
	{
		if (i->id() == achievement)
		{
			*i = achievement_data(achievement, bitmask);
			return true;
		}
	}

	_achievements.push_back(achievement_data(achievement, bitmask));
	return true;
}

void achievements::update(const u_int8 & achievement, const u_int8 & bit)
{
	for (vector<achievement_data>::iterator i = _achievements.begin(); i != _achievements.end(); i++)
	{
		if (i->id() == achievement)
		{
			bool unlocked = i->update(bit);
			if (unlocked)
			{
				// immediately update global achievement cache
				make_persistent();

				// notify listener that achievement was unlocked
				if (_callback)
				{
				    _callback->callback_func1 (i->id());
				}
			}
			break;
		}
	}
}

int achievements::num_unlocked ()
{
	int result = 0;
	for (vector<achievement_data>::iterator i = _achievements.begin(); i != _achievements.end(); i++)
	{
		if (i->is_unlocked())
		{
			result++;
		}
	}
	return result;
}

bool achievements::is_unlocked (const u_int32 & index)
{
	if (index < _achievements.size())
	{
		return _achievements[index].is_unlocked();
	}
	return false;
}

u_int8 achievements::achievement_id(const u_int32 & index)
{
	if (index < _achievements.size())
	{
		return _achievements[index].id();
	}
	return 0;
}

bool achievements::get_state (igzstream& file)
{
	// if achievements are loaded already, only update current value
	bool update_only = _achievements.size() > 0;

	u_int8 id;
	u_int32 j, expected, current;
	j << file;

	for (u_int32 i = 0; i < j; i++)
	{
		id << file;
		expected << file;

		if (update_only)
		{
			current << file;
			for (vector<achievement_data>::iterator i = _achievements.begin(); i != _achievements.end(); i++)
			{
				if (i->id() == id)
				{
					i->_current = current;
					break;
				}
			}
		}
		else
		{
			_achievements.push_back(achievement_data(id, expected));
			_achievements[i]._current << file;
		}
	}

	return true;
}

void achievements::put_state (ogzstream& file)
{
	u_int32 j;

    j = _achievements.size ();
    j >> file;

	for (vector<achievement_data>::iterator i = _achievements.begin(); i != _achievements.end(); i++)
	{
		i->_id >> file;
		i->_expected >> file;
		i->_current >> file;
	}
}

void achievements::init()
{
    igzstream in;
    string filepath;

    // initialize achievements data from game data directory
	if (_achievements.size() == 0)
	{
		if (!gamedata::load_achievements(0))
	        return;
	}

    // Load global achievement state
    filepath = game::user_data_dir();
    filepath += "/achievement.data";

    // initially, global achievement data does not exist
    if (!in.open (filepath))
    {
    	// --> try to create it in that case
    	make_persistent();
    	return;
    }

	u_int8 id;
	u_int32 j, global;
	Bytef buffer[5];
	uLong checksum = adler32(0L, Z_NULL, 0);

	j << in;
	for (u_int32 i = 0; i < j; i++)
	{
		id << in;
		global << in;

		for (vector<achievement_data>::iterator i = _achievements.begin(); i != _achievements.end(); i++)
		{
			if (i->id() == id)
			{
				i->_persistent = global;
				break;
			}
		}

		// update checksum
		buffer[4] = id;
		memcpy(buffer, &global, 4);
		checksum = adler32(checksum, buffer, 5);
	}

	u_int32 previous_checksum;
	previous_checksum << in;

	if (previous_checksum != checksum)
	{
		cout << "Checksum error: " << previous_checksum << " != " << checksum << endl;

		_achievements.clear();
		_achievements.push_back(achievement_data(255, 0x50554e4b));
		_achievements[0]._persistent = _achievements[0]._expected;
	}

    in.close ();
}

void achievements::make_persistent()
{
    ogzstream file;
    string filepath;

    filepath = game::user_data_dir();
    filepath += "/achievement.data";

	// initially, global achievement data does not exist
	if (!file.open (filepath))
	{
		cerr << "Failed writing " << filepath << endl;
		return;
	}


	num_achievements() >> file;

	Bytef buffer[5];
	uLong checksum = adler32(0L, Z_NULL, 0);

	for (vector<achievement_data>::iterator i = _achievements.begin(); i != _achievements.end(); i++)
	{
		i->_id >> file;
		i->_persistent >> file;

		// update checksum
		buffer[4] = i->_id;
		memcpy(buffer, &i->_persistent, 4);
		checksum = adler32(checksum, buffer, 5);
	}

	((u_int32) checksum) >> file;
	file.close();
}

void achievements::py_signal_connect (PyObject *pyfunc, PyObject *args)
{
	if (_callback)
	{
		delete _callback;
	}
	_callback = new py_callback (pyfunc, args);
}


achievement_data::achievement_data(const u_int8 & id, const u_int32 & expected) :
	_id(id), _expected(expected)
{
	_current = 0;
	_persistent = 0;
}

achievement_data::~achievement_data() { }

bool achievement_data::update(const u_int8 & bit)
{
	// only ever unlock achievement once
	if (!is_unlocked() && bit < 32)
	{
		_current |= (1 << bit);

		// unlock achievement if expected value is reached
		if (_current == _expected)
		{
			_persistent = _current;
			return true;
		}
	}

	return false;
}

