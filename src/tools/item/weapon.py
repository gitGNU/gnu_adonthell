import itemtest

class weapon:
    def use (self, a_weapon):
#	if a_weapon.get_val ("type") != itemtest.WEAPON: return
        print "To battle!"

    def equip (self, a_weapon, a_person):
#	if a_weapon.get_val ("type") != itemtest.WEAPON: return
	print a_person.get_name (), "equips", a_weapon.get_name ()
