#include "atk_theme.h"


atk_theme::atk_theme ()
{
    
}


void atk_theme::load (const std::string & filename)
{
    /* init hash_map border */
    destroy (); 
    
    /* open file with gz stream */
    igzstream fd;
    
    if (!fd.open (filename))
    {
        std::cerr << "error atk_theme::load " <<  filename << std::endl;
        return; 
    }

    /* load name */
    name_ <<  fd; 
    
    /* start to read number of border */
    u_int8 nb_border;
    nb_border << fd; 

    // std::cout << nb_border << std::endl; 
    
    atk_border_template * tmp; 
    for (u_int8 i = 0; i < nb_border; i++)
    {
        tmp =  new atk_border_template ();
        tmp->load (fd);
        add_border (tmp); 
    }
    
    /* close the fd */
    fd.close (); 
}



void atk_theme::save (const std::string & filename)
{
    /* open a gz output stream */
    ogzstream fd;
    if (!fd.open (filename))
    {
        std::cerr << "error atk_theme::save " <<  filename << std::endl;
        return;  
    }

    /* save name */
    name_ >> fd; 
    
    /* save number of border */
    u_int8 nb_border = border_.size ();
    nb_border >> fd;

    for (std::hash_map <std::string, atk_border_template * >::iterator it = border_.begin (); it  != border_.end (); it++)
        it->second->save (fd); 
    
    fd.close (); 
}


std::string atk_theme::get_name () const
{
    return name_; 
}


void atk_theme::set_name (const std::string & name)
{
    name_ =  name; 
}


void atk_theme::add_border (atk_border_template * border)
{
    border_[border->get_name () ] =  border; 
}


void atk_theme::remove_border (const std::string & name)
{
    std::hash_map <std::string, atk_border_template * >::iterator it = border_.find (name);
    if ( it ==  border_.end ()) return;
    delete it->second;
    border_.erase (it); 
}


void atk_theme::destroy ()
{
     // free border
    for (std::hash_map <std::string, atk_border_template * >::iterator it = border_.begin (); it  != border_.end (); it++)
        delete it->second;
    border_.clear (); 
}


atk_theme::~atk_theme ()
{
    destroy (); 
}



void atk_theme::display_info () 
{
    std::cout << "Theme name : " << name_ << std::endl;
    std::cout << "List of border : \n"; 

     
    for (std::hash_map <std::string, atk_border_template * >::iterator it = border_.begin (); it  != border_.end (); it++)
        it->second->display_info (); 
}





