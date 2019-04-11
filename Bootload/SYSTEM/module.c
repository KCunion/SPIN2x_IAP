#include "sys.h"
#include "module.h"

//evevt driven
void set_event(event_t *ptEvent)
{
    if (NULL != ptEvent) {
        ptEvent->bIsSet = true;
    }
}

bool wait_event(event_t *ptEvent)
{
    if (NULL != ptEvent) {
        if (false != ptEvent->bIsSet) {
            if (false != ptEvent->bAutoReset) {
                ptEvent->bIsSet = false;
            }
            return true;
        }
    }
    return false;
}

void reset_event(event_t *ptEvent)
{
    if (NULL != ptEvent) {
        ptEvent->bIsSet = false;
    }
}

void init_event(event_t *ptEvent,bool bValue,bool bAuto)
{
    if (NULL != ptEvent) {
        ptEvent->bAutoReset = bAuto;
        ptEvent->bIsSet = bValue;
    }
}

//mailbox
void init_mail(mailbox_t *ptMail)
{
    if (NULL != ptMail) {
        INIT_EVENT(&ptMail->tSealed,RESET,AUTO);
        ptMail->pTarget = NULL;
    }
}

void *open_mail(mailbox_t *ptMail)
{
    if (NULL != ptMail) {
        if (WAIT_EVENT(&ptMail ->tSealed)) {
            return ptMail->pTarget;
        }
    }
    return NULL;
}

void post_mail(mailbox_t *ptMail,void *pTarget)
{
    if (NULL != ptMail && NULL != pTarget) {
        SET_EVENT(&ptMail ->tSealed);
        ptMail->pTarget = pTarget;
    }
}

//critical Section
bool enter_cricital_sector(critical_sector_t *ptCritical)
{
    if (NULL != ptCritical) {
        if (false == ptCritical ->bLocked) {
            ptCritical ->bLocked = true;
            return true;
        }
    }
    return false;
}

void leave_cricital_sector(critical_sector_t *ptCritical)
{
    if (NULL != ptCritical) {
        ptCritical ->bLocked = false;
    }
}

void init_cricital_sector(critical_sector_t *ptCritical)
{
    if (NULL != ptCritical) {
        ptCritical ->bLocked = false;
    }
}
