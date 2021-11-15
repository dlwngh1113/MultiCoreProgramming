#include "LockFreeExchanger.h"

int LockFreeExchanger::Exchange(int value, bool* timeOut, bool* busy)
{
    while (true)
    {
        unsigned int curSlot = slot;
        int state = curSlot >> 30;
        switch (state)
        {
        case EMPTY:
        {
            unsigned int newVal = value | (WAITING << 30);
            if (std::atomic_compare_exchange_strong(&slot, &curSlot, newVal))
            {
                for (int i = 0; i < 10; ++i)
                {
                    if (BUSY == (slot >> 30))
                    {
                        int retValue = slot & 0xCFFFFFFF;
                        slot = 0;
                        return retValue;
                    }
                }
                curSlot = slot;
                if (std::atomic_compare_exchange_strong(&slot, &curSlot, 0))
                {
                    *timeOut = true;
                    *busy = false;
                    return 0;
                }
                else
                {
                    int retValue = slot & 0xCFFFFFFF;
                    slot = 0;
                    return retValue;
                }
            }
            else
                continue;
        }
        case WAITING:
        {
            int newVal = value | (BUSY << 30);
            if (std::atomic_compare_exchange_strong(&slot, &curSlot, newVal))
                return (curSlot & 0xCFFFFFFF);
            else
            {
                *timeOut = false;
                *busy = true;
                return 0;
            }
        }
        case BUSY:
            *timeOut = false;
            *busy = true;
            return 0;
        }
    }
}
