#ifndef CERTI_OBJECT_CLASS_1516e_HH
#define CERTI_OBJECT_CLASS_1516e_HH

#include "../ObjectClass.hh"
#include "ObjectClassAttribute1516e.h"

#include <cassert>

namespace certi {


class ObjectClass1516e : public ObjectClass
{
public:

    ObjectClass1516e()=delete;

    /** Create an objectClass.
     * @param[in] name the object class name
     * @param[in] handle the object class handle value
     */
    ObjectClass1516e(const std::string& name, ObjectClassHandle handle) : ObjectClass(name, handle){}

    /// Destroy an object class.
    virtual ~ObjectClass1516e(){}

    void addInheritedClassAttributes(ObjectClass* the_child)
    {
        for (HandleClassAttributeMap::iterator a = _handleClassAttributeMap.begin(); a != _handleClassAttributeMap.end();
             ++a)
        {
            assert(a->second != NULL);

            ObjectClassAttribute1516e* childAttribute = new ObjectClassAttribute1516e(*static_cast<ObjectClassAttribute1516e*>(a->second));
//            ObjectClassAttribute* childAttribute = new ObjectClassAttribute(*a->second);
            assert(childAttribute != NULL);

            the_child->addAttribute(childAttribute, true);
        }
    }



};

}
#endif
