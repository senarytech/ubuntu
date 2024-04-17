/*
 * TrustZone Driver
 *
 * Copyright (C) 2013 Marvell Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include <linux/mm.h>
#include <linux/uaccess.h>

/*
 * This function walks through the page tables to convert a userland
 * virtual address to a page table entry (PTE)
 */
unsigned long tz_user_virt_to_pte(struct mm_struct *mm, unsigned long address)
{
	uint32_t tmp;
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *ptep, pte;
#ifdef pte_offset_map_lock
	spinlock_t *lock;
#endif

	/* va in user space might not be mapped yet, so do a dummy read here
	 * to trigger a page fault and tell kernel to create the revalant
	 * pte entry in the page table for it. It is possible to fail if
	 * the given va is not a valid one
	 */
	if (get_user(tmp, (uint32_t __user *)address))
		return 0;

	pgd = pgd_offset(mm, address);
	if (!pgd_present(*pgd))
		return 0;

	pud = pud_offset(pgd, address);
	if (!pud_present(*pud))
		return 0;

	pmd = pmd_offset(pud, address);
	if (!pmd_present(*pmd))
		return 0;

#ifdef pte_offset_map_lock
	ptep = pte_offset_map_lock(mm, pmd, address, &lock);
#else
	ptep = pte_offset_map(pmd, address);
#endif
	if (!ptep)
		return 0;
	pte = *ptep;
#ifdef pte_offset_map_lock
	pte_unmap_unlock(pte, lock);
#endif

	if (pte_present(pte))
		return pte_val(pte) & PHYS_MASK;

	return 0;
}

/*
 * This function walks through the page tables to convert a userland
 * virtual address to physical address
 */
unsigned long tz_user_virt_to_phys(struct mm_struct *mm, unsigned long address)
{
	unsigned long pte = tz_user_virt_to_pte(mm, address);
	return pte & PAGE_MASK;
}
