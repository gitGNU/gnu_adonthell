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
 * @file achievements.h
 *
 * @author Kai Sterker
 * @brief Manages in-game achievements
 */

#ifndef ACHIEVEMENTS_H_
#define ACHIEVEMENTS_H_

#include "py_callback.h"
#include "fileops.h"
#include <vector>

using namespace std;

#ifndef SWIG

/**
 * Data for a single achievement. This contains of a unique id,
 * the state that will unlock the achievement, whether the
 * achievement has already been unlocked and the state of the
 * achievement in the current game.
 */
class achievement_data
{
public:
	/**
	 * Create a new achievement with the given id and the bitmask
	 * that will eventually unlock it.

	 * @param id the unique achievement id
	 * @param expected the value that will unlock the achievement
	 */
    achievement_data(const u_int8 & id, const u_int32 & expected);
	virtual ~achievement_data();

	/**
	 * Get the unique id of the achievement
	 * @return the unique id of the achievement
	 */
	u_int8 id() const { return _id; }

	/**
	 * Set the nth bit of the given achievement to 1.
	 * @param bit the index of the bit to set to 1
	 * @return \e true if the new achievement value matches the expected
	 * 	bitmask to permanently unlock the achievement. \e False otherwise.
	 */
	bool update(const u_int8 & bit);

	/**
	 * Check whether the achievement is permanently unlocked
	 * @return \e true if the achievement is unlocked, \e false otherwise.
	 */
	bool is_unlocked() const { return _persistent == _expected; }

	friend class achievements;

private:
	/// the unique id
	u_int8 _id;
	/// expected value to unlock this achievement
	u_int32 _expected;
	/// value of the achievement for the current game
	u_int32 _current;
	/// value of the achievement for all games
	u_int32 _persistent;
};

#endif

/**
 * Class that keeps track of all available achievements,
 * their permanent unlocked state and state in the current game.
 */
class achievements
{
public:
	/**
	 * Create a new achievement with the given id and the bitmask
	 * that will eventually unlock it. If an achievement with the
	 * given id already exists, it will be updated.
	 *
	 * Note that ids 0 and 255 are reserved.
	 *
	 * @param achievement the unique achievement id
	 * @param bitmask the value that will unlock the achievement
	 *
	 * @return \e true on success, \e false otherwise
	 */
	static bool create(const u_int8 & achievement, const u_int32 & bitmask);

	/**
	 * Set the nth bit of the given achievement to 1.
	 * If the current value of the achievement afterwards matches
	 * the bitmask given when creating the achievement, the achievement
	 * will be unlocked permanently (only once).
	 *
	 * @param achievement the unique achievement id
	 * @param bit the index of the bit to set to 1.
	 */
	static void update(const u_int8 & achievement, const u_int8 & bit);

	/**
	 * Return the total number of available achievements.
	 * @returns the number of achievements.
	 */
	static int num_achievements() { return _achievements.size(); }

	/**
	 * Returns how many achievements have been permanently unlocked.
	 * @returns the number of unlocked achievements.
	 */
	static int num_unlocked ();

	/**
	 * Checks whether the achievement at the given index is unlocked.
	 * @param index a number between 0 and num_achievements
	 * @return \e true if the achievement has been unlocked, \e false if not.
	 */
	static bool is_unlocked (const u_int32 & index);

	/**
	 * Get the achievement id at the given index.
	 * @param index a number between 0 and num_achievements
	 * @return the id of the achievement stored at the given index.
	 */
	static u_int8 achievement_id(const u_int32 & index);

	/**
	 * Allow to connect a python callback to get notified when a new
	 * achievement was unlocked. Callback will receive the achievement id
	 * along with any additional arguments given when connecting the callback
     *
	 * @param pyfunc the callback to register
	 * @param args the optional arguments to use when executing the callback
	 */
	static void py_signal_connect (PyObject *pyfunc, PyObject *args = NULL);

#ifndef SWIG
	/**
	 * Load achievement data from stream.
	 * @param file the stream to read data from.
	 * @return \e true on success, \e false otherwise.
	 */
	static bool get_state (igzstream& file);

	/**
	 * Save achievement data to stream.
	 * @param file the stream to write data to.
	 */
	static void put_state (ogzstream& file);

	/**
	 * Initialize achievements by loading all available
	 * achievements and their permanent unlocked status.
	 */
	static void init();

	/**
	 * Write permanent unlock status of available achievements.
	 */
	static void make_persistent();
#endif

private:
	achievements();
	virtual ~achievements();

#ifndef SWIG
	/// a callback executed when an achievement gets unlocked
	static py_callback *_callback;

	/// list of available achievements
	static vector<achievement_data> _achievements;
#endif
};

#endif
