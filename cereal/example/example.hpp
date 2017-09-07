#ifndef EXAMPLE_HPP_
#define EXAMPLE_HPP_

#include <cereal/cereal.hpp>    // for serialize
                                // CEREAL_CLASS_VERSION
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>  // for smart pointer serialization
#include <cereal/types/string.hpp>  // for string serialization
#include <cereal/access.hpp>    // for cereal::access 
                                // load_and_construct
                                // specialize

// need to be included before registering derived classes; see below
#include <cereal/archives/xml.hpp>
#include <cereal/archives/binary.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include <iterator>


/* AbstractBase */
/* ------------------------------------------------------------------------- */
class AbstractBase
{
    // friend class cereal::access; // not needed?
public:
    virtual void info() const = 0;
protected:
    virtual ~AbstractBase() = default;
};
/* ------------------------------------------------------------------------- */

/* Base */
/* ------------------------------------------------------------------------- */
class Base : public AbstractBase
{
    friend class cereal::access;
public:
    Base() = default;
    Base(const std::string& name, long num)
        : name_(name), num_(num)
        { }
    Base(const Base&) = default;
    Base(Base&&) = default;
    Base& operator=(const Base&) = default;
    Base& operator=(Base&&) = default;
    virtual ~Base() = default;

    virtual void info() const;
    virtual Base* clone() const &
        {
            return new Base(*this);
        }
    virtual Base* clone() &&
        {
            return new Base(std::move(*this));
        }

protected:

    void do_info() const;

private:
// Serialization
// ---
    // serialization template function
    template<typename Archive>
    void serialize( Archive& ar, const std::uint32_t version )
        {
            ar( CEREAL_NVP(name_),  // use the same name as the variable
                num_ );             // no name specified - will generate an enumerated one
            if(version > 2)
            std::cout << "version > 2. We could do something if this was real code";
        }
// ---

// --- member variables
    std::string name_;
    long num_;
};
// Explicitly specify version number
CEREAL_CLASS_VERSION( Base, 0 )
/* ------------------------------------------------------------------------- */


/* DerivedOne */
/* ------------------------------------------------------------------------- */
class DerivedOne : virtual public Base
{
    friend class cereal::access;
public:
    DerivedOne() = default;
    DerivedOne(const std::string& name, long num)
        : Base(name, num)
        , sps_(std::make_shared<std::string>(name + " DerivedOne"))
        { }
    DerivedOne(const DerivedOne&) = default;
    DerivedOne(DerivedOne&&) = default;
    DerivedOne& operator=(const DerivedOne&) = default;
    DerivedOne& operator=(DerivedOne&&) = default;
    virtual ~DerivedOne() = default;
    
    virtual void info() const;
    virtual DerivedOne* clone() const &
        { return new DerivedOne(*this); }
    virtual DerivedOne* clone() &&
        { return new DerivedOne(std::move(*this)); }
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
            ar( cereal::virtual_base_class<Base>(this),
                // cereal::make_nvp("shared_DerivedOne",sps_ ); // serialize the member variable
                sps_);
            
            if(version > 2)
            std::cout << "version > 2. We could do something if this was real code";
        }

    template<typename Archive>
    void save( Archive& ar, const std::uint32_t version ) const
        {
                // serialize the base class
                // for non-virtual inheritance use cereal::base_class
            ar( cereal::virtual_base_class<Base>(this),
                cereal::make_nvp("shared_DerivedOne",sps_ ) ); // serialize the member variable
            
            if(version > 2)
            std::cout << "version > 2. We could do something if this was real code";
        }

    // the class has no default constructor - we need to overload 
    // the load_and_construct
    // template<typename Archive>
    // static void load_and_construct( Archive& ar,
    //                                 cereal::construct<DerivedOne>& construct,
    //                                 const std::uint32_t version)
    //     {
    //         std::shared_ptr<std::string> sps;
    //         std::string name;
    //         long num;
    //         ar( sps, name, num );
    //         construct( name, num ); // calls DerivedOne( name, num );
            
    //         if(version > 2)
    //         std::cout << "version > 2. We could do something if this was real code";
    //     }
// ---

    std::shared_ptr<std::string> sps_;
};
CEREAL_CLASS_VERSION( DerivedOne, 0 )

// ---
// DerivedOne inherits the serialize() template function from Base
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

/* ------------------------------------------------------------------------- */


/* DerivedTwo */
/* ------------------------------------------------------------------------- */
class DerivedTwo : virtual public Base
{
    friend class cereal::access;
public:
    DerivedTwo() = default;
    DerivedTwo(const std::string& name, long num)
        : Base(name, num)
        , sps_(std::make_shared<std::string>(name + " DerivedTwo"))
        { }
    DerivedTwo(const DerivedTwo&) = default;
    DerivedTwo(DerivedTwo&&) = default;
    DerivedTwo& operator=(const DerivedTwo&) = default;
    DerivedTwo& operator=(DerivedTwo&&) = default;
    virtual ~DerivedTwo() = default;

    virtual void info() const;
    virtual DerivedTwo* clone() const &
        { return new DerivedTwo(*this); }
    virtual DerivedTwo* clone() &&
        { return new DerivedTwo(std::move(*this)); }

protected:
    void do_info() const;
private:

// Serialization
// ---
    template<typename Archive>
    void serialize( Archive& ar, const std::uint32_t version )
        {
            ar( cereal::virtual_base_class<Base>( this ),
                cereal::make_nvp("shared_DerivedTwo",sps_ ) );

            if(version > 2)
            std::cout << "version > 2. We could do something if this was real code";
        }

    // template<typename Archive>
    // static void load_and_construct( Archive& ar,
    //                                 cereal::construct<DerivedTwo>& construct,
    //                                 const std::uint32_t version)
    //     {
    //         std::string name;
    //         long num;
    //         ar( name, num );
    //         construct( name, num ); // calls DerivedTwo( name, num );
            
    //         if(version > 2)
    //         std::cout << "version > 2. We could do something if this was real code";
    //     }
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
    DerivedMultiple() = default;
    DerivedMultiple(const std::string& name, long num)
        : Base(name, num)
        , DerivedOne(name, num)
        , DerivedTwo(name, num)
        { }
    DerivedMultiple(const DerivedMultiple&) = default;
    DerivedMultiple(DerivedMultiple&&) = default;
    DerivedMultiple& operator=(const DerivedMultiple&) = default;
    DerivedMultiple& operator=(DerivedMultiple&&) = default;
    virtual ~DerivedMultiple() = default;

    virtual void info() const;
    virtual DerivedMultiple* clone() const &
        { return new DerivedMultiple(*this); }
    virtual DerivedMultiple* clone() &&
        { return new DerivedMultiple(std::move(*this)); }
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

        if(version > 2)
            std::cout << "version > 2. We could do something if this was real code";
    }

    template<typename Archive>
    void save( Archive& ar, const std::uint32_t version ) const
    {
        ar( cereal::virtual_base_class<DerivedOne>( this ),
            cereal::virtual_base_class<DerivedTwo>( this ));

        if(version > 2)
        std::cout << "version > 2. We could do something if this was real code";
    }

    // This class has no default ctor. We need to overload the
    // load_and_construct() static function
    // template<typename Archive>
    // static void load_and_construct( Archive& ar,
    //                                 cereal::construct<DerivedMultiple>& construct,
    //                                 const std::uint32_t version)
    // {
    //     std::string name;
    //     long num;
    //     ar( name, num );
    //     construct( name, num ); // will call DerivedMultiple(name, num)
        
    //     if(version > 2)
    //     std::cout << "version > 2. We could do something if this was real code";
    // }
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
class Blob
{
    friend class cereal::access;
    friend std::ostream& operator<<(std::ostream& os, const Blob& b);
public:
    Blob() = default;
    void push_back(const Base& b)
    {
        shared_data.emplace_back(b.clone());
    }
    void push_back(Base&& rv)
    {
        shared_data.emplace_back(std::move(rv).clone());
    }

// iterators
    // typedef std::shared_ptr<Base>* iterator;
    // typedef const std::shared_ptr<Base>* const_iterator;
    typedef std::vector<std::shared_ptr<Base>>::iterator iterator;
    typedef std::vector<std::shared_ptr<Base>>::const_iterator const_iterator;
    iterator begin()
        { return shared_data.begin(); }
    iterator end()
        { return shared_data.end(); }
    const_iterator cbegin() const noexcept
        { return shared_data.cbegin(); }
    const_iterator cend() const noexcept
        { return shared_data.cend(); }
    
    template<typename Archive>
    void serialize( Archive& ar, const std::uint32_t version )
    {
        ar( cereal::make_nvp("Blob",  shared_data) );
        if(version > 2)
            std::cout << "version > 2; we could do something interesting if this were real code";
    }
private:
    std::vector<std::shared_ptr<Base>> shared_data;
};
inline std::ostream& operator<<(std::ostream& os, const Blob& b)
{
    for(std::vector<std::shared_ptr<Base>>::const_iterator it(b.shared_data.cbegin());
        it != b.shared_data.cend(); /*  */){
            (*it)->info();
        }
    return os;
}
CEREAL_CLASS_VERSION( Blob, 0 )
/* ------------------------------------------------------------------------- */

CEREAL_REGISTER_TYPE( Base )    /* Base is actually derived from AbstractBase */
CEREAL_REGISTER_TYPE( DerivedOne )
CEREAL_REGISTER_TYPE( DerivedTwo )
CEREAL_REGISTER_TYPE( DerivedMultiple )
CEREAL_REGISTER_POLYMORPHIC_RELATION( AbstractBase, Base )

// ---
/* All derived classes need to be registered explicitly
 * CEREAL_REGISTER_TYPE( Derived )
 * or
 * CEREAL_REGISTER_TYPE_WITH_NAME( Derived, "SomeName" )
 * The registration needs to be done AFTER any archive headers are included
 *
 * The polymorphic relation to the base class needs to be registered.
 * This is normally done automatically (as in this case) with the call to
 * cereal::base_class or cereal::virtual_base_class
 * if the base class isn't serialized (eg. pure abstract - with no members)
 * the polymorphic relation needs to be explicitly specified with
 * CEREAL_REGISTER_POLYMORPHIC_RELATION( AbstractBase, Derived )
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

#endif /* EXAMPLE_HPP_ */
