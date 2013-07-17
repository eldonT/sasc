/*
 *	dvblb_proc.c
 *
 *	DVBLoopback Copyright Alan Nisota 2006
 *
 *	Portions of this code also have copyright:
 *	Video Loopback Copyright Jeroen Vreeken (pe1rxq@amsat.org), 2000
 *
 *	Published under the GNU Public License.
 *	DVBLoopback is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	DVBLoopback is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Foobar; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <linux/version.h>      /* >= 2.6.14 LINUX_VERSION_CODE */
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/proc_fs.h>
#include "dvblb_internal.h"
#include "internal.h"

static struct proc_dir_entry *procdir;

int dvblb_remove_procfs(struct proc_dir_entry *pdir,
                        struct proc_dir_entry *parent)
{
	char name[20];
	memcpy(name, pdir->name, pdir->namelen);
	name[pdir->namelen] = '\0';
	// printk("Removing proc: %s\n", name);
	remove_proc_entry(name, parent);
	return 0;
}
EXPORT_SYMBOL(dvblb_remove_procfs);

int dvblb_init_procfs_device(struct dvblb *dvblb, struct dvblb_devinfo *lbdev)
{
	int type = lbdev->lb_dev->type;
	lbdev->procfile = proc_create(dnames[type], 0644, dvblb->procdir);
	if (lbdev->procfile == NULL)
		return -ENOMEM;
	lbdev->procfile->data = lbdev;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)
	lbdev->procfile->owner = THIS_MODULE;
#endif
	return 0;
}
EXPORT_SYMBOL(dvblb_init_procfs_device);

int dvblb_init_procfs_adapter(struct dvblb *dvblb)
{
	char name[10];
	sprintf(name, "adapter%d", dvblb->adapter.num);
	dvblb->procdir = proc_mkdir(name, procdir);
	if (dvblb->procdir == NULL)
		return -ENOMEM;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)
	dvblb->procdir->owner = THIS_MODULE;
#endif
	dvblb->procfile = proc_create("adapter", 0644, dvblb->procdir);
	if (dvblb->procfile == NULL) {
		dvblb_remove_procfs(dvblb->procdir, procdir);
		return -ENOMEM;
	}
	dvblb->procfile->data = dvblb;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)
	dvblb->procfile->owner = THIS_MODULE;
#endif
	dvblb->init |= DVBLB_STATUS_PROC;

	return 0;
}
EXPORT_SYMBOL(dvblb_init_procfs_adapter);

int dvblb_remove_procfs_adapter(struct dvblb *dvblb)
{
	if(dvblb->init & DVBLB_STATUS_PROC) {
		dvblb_remove_procfs(dvblb->procfile, dvblb->procdir);
		dvblb_remove_procfs(dvblb->procdir, procdir);
		printk("removing dvblb proc adapter\n");
	}
	dvblb->init &= ~DVBLB_STATUS_PROC;
	printk("dvblb init = %x\n", dvblb->init);
	return 0;
}
EXPORT_SYMBOL(dvblb_remove_procfs_adapter);

int dvblb_init_procfs(void)
{
	procdir = proc_mkdir("dvbloopback", NULL);
	if (procdir == NULL)
		return -ENOMEM;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)
	procdir->owner = THIS_MODULE;
#endif
	return 0;
}
EXPORT_SYMBOL(dvblb_init_procfs);

int dvblb_uninit_procfs(void)
{
	dvblb_remove_procfs(procdir, NULL);
	return 0;
}
EXPORT_SYMBOL(dvblb_uninit_procfs);

