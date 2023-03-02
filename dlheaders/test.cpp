#include "merlib.h"
#include <memory>
extern "C"
{
    using namespace Mer;
    Mem::Object add(const std::vector<Mem::Object> &args)
    {
        int a = Mem::Int::get_val(args[0]);
        int b = Mem::Int::get_val(args[1]);

        return std::make_shared<Mem::Int>(a + b);
    }
}