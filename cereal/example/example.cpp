#include "example.hpp"


/* Base */
/* ------------------------------------------------------------------------- */
void Base::do_info() const
{
    std::cout << name_ << ", " << num_ << std::endl;
}
void Base::info() const
{
    std::cout << "Base:\n";
    do_info();
}
/* ------------------------------------------------------------------------- */

/* DerivedOne */
/* ------------------------------------------------------------------------- */
void DerivedOne::do_info() const
{
    std::cout << *sps_ << " at " << std::hex 
              << reinterpret_cast<const void*>(sps_.get()) << std::endl;
}

void DerivedOne::info() const
{
    std::cout << "DerivedOne:\n";
    Base::do_info();
    do_info();
}
/* ------------------------------------------------------------------------- */

/* DerivedTwo */
/* ------------------------------------------------------------------------- */
void DerivedTwo::do_info() const
{
    std::cout << *sps_ << " at " << std::hex
              << reinterpret_cast<const void*>(sps_.get()) << std::endl;
}

void DerivedTwo::info() const
{
    std::cout << "DerivedTwo:\n";
    Base::do_info();
    do_info();
}
/* ------------------------------------------------------------------------- */

/* DerivedMultiple */
/* ------------------------------------------------------------------------- */
void DerivedMultiple::do_info() const
{ /* do nothing */ }
void DerivedMultiple::info() const
{
    std::cout << "DerivedMultiple:\n";
    Base::do_info();
    DerivedOne::do_info();
    DerivedTwo::do_info();
    do_info();
}
/* ------------------------------------------------------------------------- */