#include "../includes/branch_loop.hpp"
namespace Mer
{
    namespace BranchAndLoop
    {
        void Goto::execute(char *)
        {
            *pc = *dest;
        }
    }
}