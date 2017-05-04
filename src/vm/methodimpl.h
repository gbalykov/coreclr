// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
//
// File: methodimpl.h
//


//

//
// ============================================================================

#ifndef _METHODIMPL_H 
#define _METHODIMPL_H

class MethodDesc;

// <TODO>@TODO: This is very bloated. We need to trim this down alot. However,
// we need to keep it on a 8 byte boundary.</TODO>
class MethodImpl
{
#ifdef DACCESS_COMPILE
    friend class NativeImageDumper;
#endif

    RelativePointer<PTR_DWORD>            pdwSlots;       // Maintains the slots in sorted order, the first entry is the size
    RelativePointer<DPTR( RelativePointer<PTR_MethodDesc> )> pImplementedMD;

public:

#ifndef DACCESS_COMPILE 
    ///////////////////////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        MethodDesc *m_pMD;
        MethodImpl *m_pImpl;
        DWORD       m_iCur;

    public:
        Iterator(MethodDesc *pMD);
        inline BOOL IsValid()
            { WRAPPER_NO_CONTRACT; return ((m_pImpl != NULL)&& (m_iCur < m_pImpl->GetSize())); }
        inline void Next()
            { WRAPPER_NO_CONTRACT; if (IsValid()) m_iCur++; }
        inline WORD GetSlot()
            { WRAPPER_NO_CONTRACT; CONSISTENCY_CHECK(IsValid()); _ASSERTE(FitsIn<WORD>(m_pImpl->GetSlots()[m_iCur])); return static_cast<WORD>(m_pImpl->GetSlots()[m_iCur]); }
        inline MethodDesc *GetMethodDesc()
            { WRAPPER_NO_CONTRACT; return m_pImpl->FindMethodDesc(GetSlot(), (PTR_MethodDesc) m_pMD); }
    };

#endif // !DACCESS_COMPILE

    // In DAC builds this function is meant to be called only for structures, which are not part of other structures
    inline DPTR(RelativePointer<PTR_MethodDesc>) GetImplementedMDs()
    {
        CONTRACTL {
            NOTHROW;
            GC_NOTRIGGER;
            PRECONDITION(CheckPointer(this));
        } CONTRACTL_END;
        return pImplementedMD.GetValueMaybeNull(dac_cast<TADDR>(this) + offsetof(MethodImpl, pImplementedMD));
    }

    // In DAC builds this function is meant to be called only for structures, which are not part of other structures
    inline DPTR(RelativePointer<PTR_MethodDesc>) GetImplementedMDsNonNull()
    {
        CONTRACTL {
            NOTHROW;
            GC_NOTRIGGER;
            PRECONDITION(CheckPointer(this));
        } CONTRACTL_END;
        return pImplementedMD.GetValue(dac_cast<TADDR>(this) + offsetof(MethodImpl, pImplementedMD));
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    inline DWORD GetSize()
    {
        CONTRACTL {
            NOTHROW;
            GC_NOTRIGGER;
            PRECONDITION(CheckPointer(this));
        } CONTRACTL_END;

        PTR_DWORD pSlots = GetSlots();

        if(pSlots == NULL)
            return 0;
        else
            return *pSlots;
    }

    // In DAC builds this function is meant to be called only for structures, which are not part of other structures
    inline PTR_DWORD GetSlots()
    {
        CONTRACTL {
            NOTHROW;
            GC_NOTRIGGER;
            PRECONDITION(CheckPointer(this));
            SUPPORTS_DAC;
        } CONTRACTL_END;

        PTR_DWORD pSlots = pdwSlots.GetValueMaybeNull(dac_cast<TADDR>(this) + offsetof(MethodImpl, pdwSlots));

        if(pSlots == NULL)
            return NULL;
        else
            return pSlots + 1;
    }

#ifndef DACCESS_COMPILE 

    ///////////////////////////////////////////////////////////////////////////////////////
    void SetSize(LoaderHeap *pHeap, AllocMemTracker *pamTracker, DWORD size);

    ///////////////////////////////////////////////////////////////////////////////////////
    void SetData(DWORD* slots, RelativePointer<MethodDesc*> * md);

#endif // !DACCESS_COMPILE

#ifdef DACCESS_COMPILE 
    void EnumMemoryRegions(CLRDataEnumMemoryFlags flags);
#endif

#ifdef FEATURE_PREJIT 
    void Save(DataImage *image);
    void Fixup(DataImage *image, PVOID p, SSIZE_T offset);
#endif // FEATURE_PREJIT


    // Returns the method desc for the replaced slot;
    PTR_MethodDesc FindMethodDesc(DWORD slot, PTR_MethodDesc defaultReturn);

private:
    static const DWORD INVALID_INDEX = (DWORD)(-1);
    DWORD FindSlotIndex(DWORD slot);
#ifndef DACCESS_COMPILE 
    MethodDesc* RestoreSlot(DWORD slotIndex, MethodTable *pMT);
#endif

};

#endif // !_METHODIMPL_H
