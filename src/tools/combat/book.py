import itemtest

class book:
    def use (self, a_book):
	if a_book.get_val ("type") != itemtest.BOOK: return

	print "Reading:", a_book.get_name (), "..."
	print a_book.get_description (), "\n"
