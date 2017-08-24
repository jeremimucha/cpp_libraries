#ifndef EXAMPLE_HPP_
#define EXAMPLE_HPP_

#include <cereal/cereal.hpp>    // for serialize
                                // CEREAL_CLASS_VERSION
#include <cereal/types/polymorphic.hpp>
#include <cereal/access.hpp>    // for cereal::access 
                                // load_and_construct
                                // specialize
#include <cereal/types/memory.hpp>  // for smart pointer serialization
#include <string>
#include <iostream>
#include <memory>


/* AbstractBase */
/* ------------------------------------------------------------------------- */
class AbstractBase
{
    friend class cereal::access;
public:
    AbstractBase()
        : name_(""), num_(0)
        { }
    AbstractBase(const std::string& name, long num)
        : name_(name), num_(num)
        { }
    virtual void info() const = 0;
protected:
    void do_info() const;
    virtual ~AbstractBase() = default;
private:

// Serialization
// ---
    // serialize template function
    template<typename Archive>
    void serialize( Archive& ar, const std::uint32_t version )
        {
            ar( name_, num_ );
        }
// ---

    std::string name_;
    long num_;
};

// Explicitly specify version number
CEREAL_CLASS_VERSION( AbstractBase, 0 )
/* ------------------------------------------------------------------------- */


/* DerivedOne */
/* ------------------------------------------------------------------------- */
class DerivedOne : virtual public AbstractBase
{
    friend class cereal::access;
public:
    DerivedOne(const std::string& name, long num)
        : AbstractBase(name, num)
        , sps_(std::make_shared<std::string>(name + " DerivedOne"))
        { }
    virtual void info() const;
    virtual ~DerivedOne() = default;
protected:
    void do_info() const;
private:

// Serialization
// ---
    // serialization functions - split into load and save
    template<typename Archive>
    void load( Archive& ar, const std::uint32_t version )
        {
                // serialize the base class
                // for non-virtual inheritance use cereal::base_class
            ar( cereal::virtual_base_class<AbstractBase>(this),
                sps_ ); // serialize the member variable
        }

    template<typename Archive>
    void save( Archive& ar, const std::uint32_t version ) const
        {
                // serialize the base class
                // for non-virtual inheritance use cereal::base_class
            ar( cereal::virtual_base_class<AbstractBase>(this),
                sps_ ); // serialize the member variable
        }

    // the class has no default constructor - we need to overload 
    // the load_and_construct
    template<typename Archive>
    static void load_and_construct( Archive& ar,
                                    cereal::construct<DerivedOne>& construct,
                                    const std::uint32_t version)
        {
            std::string name;
            long num;
            ar( name, num );
            construct( name, num ); // calls DerivedOne( name, num );
        }
// ---

    std::shared_ptr<std::string> sps_;
};
CEREAL_CLASS_VERSION( DerivedOne, 0 )

// ---
// DerivedOne inherits the serialize() template function from AbstractBase
// and defines it's own load() and save() functions.
// cereal can't disambiguate which it should call - this will result in
// compile-time error. We need to explicitly specify which serialization
// functions should be called.

// Define a specialize struct
namespace cereal
{
    template<typename Archive>
    struct specialize<Archive,
                      DerivedOne,
                      cereal::specialization::member_load_save>
    { };
}

// Or use a macro at global scope to do the same disambiguation:
// CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES( DerivedOne, cereal::specialization::member_load_save )
// ---

// ---
/* The polymorphic relation to the base class needs to be registered.
 * This is normally done automatically (as in this case) with the call to
 * cereal::base_class or cereal::virtual_base_class
 * if the base class isn't serialized (eg. pure abstract - with no members)
 * registration needs to be done explicitly by using one of the following macros
 * CEREAL_REGISTER_TYPE( DerivedOne )
 * or
 * CEREAL_REGISTER_TYPE_WITH_NAME( DerivedOne, "SomeName" )
 * The registration needs to be done AFTER any archive headers are included
 * The polymorphic relation also needs to be explicitly specified
 * CEREAL_REGISTER_POLYMORPHIC_RELATION( AbstractBase, DerivedOne )
 */
// ---

// ---
/* Check the documentation for
 * CEREAL_REGISTER_DYNAMIC_INIT()
 * and
 * CEREAL_FORCE_DYNAMIC_INIT() 
 * if plan on compiling static or dynamic libraries for code using serialization.
 */
// ---


/* ------------------------------------------------------------------------- */


/* DerivedTwo */
/* ------------------------------------------------------------------------- */
class DerivedTwo : virtual public AbstractBase
{
    friend class cereal::access;
public:
    DerivedTwo(const std::string& name, long num)
        : AbstractBase(name, num)
        , sps_(std::make_shared<std::string>(name + " DerivedTwo"))
        { }
    virtual void info() const;
    virtual ~DerivedTwo() = default;
protected:
    void do_info() const;
private:

// Serialization
// ---
    template<typename Archive>
    void serialize( Archive& ar, const std::uint32_t version )
        {
            ar( cereal::virtual_base_class<AbstractBase>( this ),
                sps_ );
        }

    template<typename Archive>
    static void load_and_construct( Archive& ar,
                                    cereal::construct<DerivedTwo>& construct,
                                    const std::uint32_t version)
        {
            std::string name;
            long num;
            ar( name, num );
            construct( name, num ); // calls DerivedTwo( name, num );
        }
// ---

    std::shared_ptr<std::string> sps_;
};

CEREAL_CLASS_VERSION( DerivedTwo, 0 )
/* ------------------------------------------------------------------------- */


/* DerivedMultiple */
/* ------------------------------------------------------------------------- */
class DerivedMultiple : public DerivedOne, public DerivedTwo
{
    friend class cereal::access;
public:
    DerivedMultiple(const std::string& name, long num)
        : AbstractBase(name, num)
        , DerivedOne(name, num)
        , DerivedTwo(name, num)
        { }
    virtual void info() const;
    virtual ~DerivedMultiple() = default;
protected:
    void do_info() const;

private:
    // Serialization
    // ---
    template<typename Archive>
    void load( Archive& ar, const std::uint32_t version )
    {
        // Serialize both base classes. Since the inheritance
        // is virtual cereal will ensure that the common base
        // is serialized only once.
        // This particular class has no members of it's own,
        // so serializing base classes is all it does
        ar( cereal::virtual_base_class<DerivedOne>( this ),
            cereal::virtual_base_class<DerivedTwo>( this ));
    }

    template<typename Archive>
    void save( Archive& ar, const std::uint32_t version ) const
    {
        ar( cereal::virtual_base_class<DerivedOne>( this ),
            cereal::virtual_base_class<DerivedTwo>( this ));
    }

    // This class has no default ctor. We need to overload the
    // load_and_construct() static function
    template<typename Archive>
    static void load_and_construct( Archive& ar,
                                    cereal::construct<DerivedMultiple>& construct,
                                    const std::uint32_t version)
    {
        std::string name;
        long num;
        ar( name, num );
        construct( name, num ); // will call DerivedMultiple(name, num)
    }
    // ---
};

CEREAL_CLASS_VERSION( DerivedMultiple, 0 )

/* Specify explicitly which serialization function to use.
DerivedMultiple inherits both serialize (from DerivedOne) and
save() and load() functions (from DerivedTwo) and defines its own
save() and load(). We need to disambiguate which one should be used.
This can be done with either a struct defined in the cereal namespace
namespace cereal
{
    template<typename Archive>
    struct specialize<Archive,
                      DerivedMultiple,
                      cereal::specialization::member_load_save>
    { };
}
or with a macro used at global scope */
CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES( DerivedMultiple,
                                    cereal::specialization::member_load_save )
/* ------------------------------------------------------------------------- */


/* Container */
/* ------------------------------------------------------------------------- */
// class Container
// {
//     friend class cereal::access;
// public:
//     Containter() = delete;  // no default ctor;
// private:
//     std::vector<std::shared_ptr<AbstractBase>> shared_data;
// };
/* ------------------------------------------------------------------------- */

#endif /* EXAMPLE_HPP_ */
