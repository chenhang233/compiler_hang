#include "function.h"

ASTnode *function_declaration()
{
    match_void();
    match_ident();
}