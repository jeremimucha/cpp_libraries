// Include the polymorphic serialization and registration mechanisms
#include <cereal/types/polymorphic.hpp>

// A pure virtual base class
struct BaseClass
{
    virtual void say_type() = 0;
};


// A class derived from BaseClass
struct DerivedClassOne : public BaseClass
{
    void say_type();

    int x;

    // cereal serialization function
    template<typename Archive>
    void serialize(Archive& ar)
        { ar( x ); }
};

// Another class derived from BaseClass
struct EmbarrassingDerivedClass : public BaseClass
{
    void say_type();

    float y;
    
    template<typename Archive>
    void serialize(Archive& ar)
        { ar( y ); }
};


/* Any archives we plan on using must be included prior to registering
 * Note that this could be done in other location so long as it was prior
 * to this file being included
 */
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>

// Register DerivedClassOne
CEREAL_REGISTER_TYPE(DerivedClassOne)

// Register EmbarassingDerivedClass with a less embarrasing name
CEREAL_REGISTER_TYPE_WITH_NAME(EmbarrassingDerivedClass, "DerivedClassTwo")

/* Note that there is no need to register the base class, only derived classes
 * However, since we did not use cereal::base_class, we need to clarify
 * the relationship 
 */
CEREAL_REGISTER_POLYMORPHIC_RELATION(BaseClass, DerivedClassOne)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BaseClass, EmbarrassingDerivedClass)
