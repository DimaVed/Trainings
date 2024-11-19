#include "comparators.h"
#include "object.h"
#include "object_holder.h"

#include <functional>
#include <optional>
#include <sstream>

namespace Runtime {

bool Equal(ObjectHolder lhs, ObjectHolder rhs)
{
    if (lhs.TryAs<Runtime::Number>()&&rhs.TryAs<Runtime::Number>())
    {
        return  lhs.TryAs<Runtime::Number>()->GetValue()==rhs.TryAs<Runtime::Number>()->GetValue();
    }
    else if (lhs.TryAs<Runtime::String>()&&rhs.TryAs<Runtime::String>())
    {
        return  lhs.TryAs<Runtime::String>()->GetValue()==rhs.TryAs<Runtime::String>()->GetValue();
    }
    else
    {
        throw  std::runtime_error ("Equal error");
    }

}

bool Less(ObjectHolder lhs, ObjectHolder rhs)
{
    if (lhs.TryAs<Runtime::Number>()&&rhs.TryAs<Runtime::Number>())
    {
        return  lhs.TryAs<Runtime::Number>()->GetValue()<rhs.TryAs<Runtime::Number>()->GetValue();
    }
    else if (lhs.TryAs<Runtime::String>()&&rhs.TryAs<Runtime::String>())
    {
        return  lhs.TryAs<Runtime::String>()->GetValue()<rhs.TryAs<Runtime::String>()->GetValue();
    }
    else
    {
        throw  std::runtime_error ("Less error");
    }
}

} /* namespace Runtime */
