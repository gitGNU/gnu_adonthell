import itemtest

class weapon:
    def use (self, a_weapon):
        print "To battle!"

    def equip (self, a_weapon, a_person):
	print a_person.get_name (), "equips", a_weapon.get_name ()
