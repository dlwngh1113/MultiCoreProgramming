#include "LockFreeExchanger.h"

int LockFreeExchanger::Exchange(int value, bool* timeOut, bool* busy)
{
    while (true)
    {
        unsigned int curSlot = slot;
        int value = curSlot & 0xCFFFFFFF;
        int state = curSlot >> 30;
        switch (state)
        {
        case EMPTY:
            unsigned int newVal = value | (WAITING << 30);
            if (std::atomic_compare_exchange_strong(&slot, &curSlot, newVal))
            {
                for (int i = 0; i < 10; ++i)
                {
                    if (BUSY == slot >> 30)
                    {
                        int retValue = slot & 0xCFFFFFFF;
                        slot = 0;
                        return retValue;
                    }
                }
                *timeOut = true;
                *busy = false;
                return 0;
            }
            else
            {
                continue;
            }
            break;
        case WAITING:
        {
            int newVal = value | (BUSY << 30);
            if (std::atomic_compare_exchange_strong(&slot, &curSlot, newVal))
            {
                return value;
            }
            else
            {
                *timeOut = false;
                *busy = true;
                return 0;
            }
        }
            break;
        case BUSY:
            *timeOut = false;
            *busy = true;
            return 0;
            break;
        }
    }
}
