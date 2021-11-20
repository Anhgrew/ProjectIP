#include "User.h"

int User::current_id = 0;
void User::printUser()
{
    std::cout << "Name: " << name << "ID: " << id << "Rank: " << rank << "Ans: " << ans << "Turn: " << turn << std::endl;
}
