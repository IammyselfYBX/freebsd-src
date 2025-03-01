/*-
 * Copyright (c) 2010 Isilon Systems, Inc.
 * Copyright (c) 2010 iX Systems, Inc.
 * Copyright (c) 2010 Panasas, Inc.
 * Copyright (c) 2013-2016 Mellanox Technologies, Ltd.
 * All rights reserved.
 * Copyright (c) 2020-2021 The FreeBSD Foundation
 *
 * Portions of this software were developed by Björn Zeeb
 * under sponsorship from the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD$
 */
#ifndef	_LINUXKPI_LINUX_PCI_H_
#define	_LINUXKPI_LINUX_PCI_H_

#define	CONFIG_PCI_MSI

#include <linux/types.h>

#include <sys/param.h>
#include <sys/bus.h>
#include <sys/nv.h>
#include <sys/pciio.h>
#include <sys/rman.h>
#include <sys/bus.h>
#include <dev/pci/pcivar.h>
#include <dev/pci/pcireg.h>
#include <dev/pci/pci_private.h>

#include <machine/resource.h>

#include <linux/list.h>
#include <linux/dmapool.h>
#include <linux/dma-mapping.h>
#include <linux/compiler.h>
#include <linux/errno.h>
#include <asm/atomic.h>
#include <linux/device.h>
#include <linux/pci_ids.h>

struct pci_device_id {
	uint32_t	vendor;
	uint32_t	device;
	uint32_t	subvendor;
	uint32_t	subdevice;
	uint32_t	class;
	uint32_t	class_mask;
	uintptr_t	driver_data;
};

#define	MODULE_DEVICE_TABLE(bus, table)

#define	PCI_ANY_ID			-1U

#define PCI_DEVFN(slot, func)   ((((slot) & 0x1f) << 3) | ((func) & 0x07))
#define PCI_SLOT(devfn)		(((devfn) >> 3) & 0x1f)
#define PCI_FUNC(devfn)		((devfn) & 0x07)
#define	PCI_BUS_NUM(devfn)	(((devfn) >> 8) & 0xff)

#define PCI_VDEVICE(_vendor, _device)					\
	    .vendor = PCI_VENDOR_ID_##_vendor, .device = (_device),	\
	    .subvendor = PCI_ANY_ID, .subdevice = PCI_ANY_ID
#define	PCI_DEVICE(_vendor, _device)					\
	    .vendor = (_vendor), .device = (_device),			\
	    .subvendor = PCI_ANY_ID, .subdevice = PCI_ANY_ID

#define	to_pci_dev(n)	container_of(n, struct pci_dev, dev)

#define	PCI_VENDOR_ID		PCIR_DEVVENDOR
#define	PCI_COMMAND		PCIR_COMMAND
#define	PCI_COMMAND_INTX_DISABLE	PCIM_CMD_INTxDIS
#define	PCI_EXP_DEVCTL		PCIER_DEVICE_CTL		/* Device Control */
#define	PCI_EXP_LNKCTL		PCIER_LINK_CTL			/* Link Control */
#define	PCI_EXP_LNKCTL_ASPM_L0S	PCIEM_LINK_CTL_ASPMC_L0S
#define	PCI_EXP_LNKCTL_ASPM_L1	PCIEM_LINK_CTL_ASPMC_L1
#define PCI_EXP_LNKCTL_ASPMC	PCIEM_LINK_CTL_ASPMC
#define	PCI_EXP_LNKCTL_CLKREQ_EN PCIEM_LINK_CTL_ECPM		/* Enable clock PM */
#define PCI_EXP_LNKCTL_HAWD	PCIEM_LINK_CTL_HAWD
#define	PCI_EXP_FLAGS_TYPE	PCIEM_FLAGS_TYPE		/* Device/Port type */
#define	PCI_EXP_DEVCAP		PCIER_DEVICE_CAP		/* Device capabilities */
#define	PCI_EXP_DEVSTA		PCIER_DEVICE_STA		/* Device Status */
#define	PCI_EXP_LNKCAP		PCIER_LINK_CAP			/* Link Capabilities */
#define	PCI_EXP_LNKSTA		PCIER_LINK_STA			/* Link Status */
#define	PCI_EXP_SLTCAP		PCIER_SLOT_CAP			/* Slot Capabilities */
#define	PCI_EXP_SLTCTL		PCIER_SLOT_CTL			/* Slot Control */
#define	PCI_EXP_SLTSTA		PCIER_SLOT_STA			/* Slot Status */
#define	PCI_EXP_RTCTL		PCIER_ROOT_CTL			/* Root Control */
#define	PCI_EXP_RTCAP		PCIER_ROOT_CAP			/* Root Capabilities */
#define	PCI_EXP_RTSTA		PCIER_ROOT_STA			/* Root Status */
#define	PCI_EXP_DEVCAP2		PCIER_DEVICE_CAP2		/* Device Capabilities 2 */
#define	PCI_EXP_DEVCTL2		PCIER_DEVICE_CTL2		/* Device Control 2 */
#define	PCI_EXP_DEVCTL2_LTR_EN	PCIEM_CTL2_LTR_ENABLE
#define	PCI_EXP_LNKCAP2		PCIER_LINK_CAP2			/* Link Capabilities 2 */
#define	PCI_EXP_LNKCTL2		PCIER_LINK_CTL2			/* Link Control 2 */
#define	PCI_EXP_LNKSTA2		PCIER_LINK_STA2			/* Link Status 2 */
#define	PCI_EXP_FLAGS		PCIER_FLAGS			/* Capabilities register */
#define	PCI_EXP_FLAGS_VERS	PCIEM_FLAGS_VERSION		/* Capability version */
#define	PCI_EXP_TYPE_ROOT_PORT	PCIEM_TYPE_ROOT_PORT		/* Root Port */
#define	PCI_EXP_TYPE_ENDPOINT	PCIEM_TYPE_ENDPOINT		/* Express Endpoint */
#define	PCI_EXP_TYPE_LEG_END	PCIEM_TYPE_LEGACY_ENDPOINT	/* Legacy Endpoint */
#define	PCI_EXP_TYPE_DOWNSTREAM PCIEM_TYPE_DOWNSTREAM_PORT	/* Downstream Port */
#define	PCI_EXP_FLAGS_SLOT	PCIEM_FLAGS_SLOT		/* Slot implemented */
#define	PCI_EXP_TYPE_RC_EC	PCIEM_TYPE_ROOT_EC		/* Root Complex Event Collector */
#define	PCI_EXP_LNKCAP_SLS_2_5GB 0x01	/* Supported Link Speed 2.5GT/s */
#define	PCI_EXP_LNKCAP_SLS_5_0GB 0x02	/* Supported Link Speed 5.0GT/s */
#define	PCI_EXP_LNKCAP_SLS_8_0GB 0x04	/* Supported Link Speed 8.0GT/s */
#define	PCI_EXP_LNKCAP_SLS_16_0GB 0x08	/* Supported Link Speed 16.0GT/s */
#define	PCI_EXP_LNKCAP_MLW	0x03f0	/* Maximum Link Width */
#define	PCI_EXP_LNKCAP2_SLS_2_5GB 0x02	/* Supported Link Speed 2.5GT/s */
#define	PCI_EXP_LNKCAP2_SLS_5_0GB 0x04	/* Supported Link Speed 5.0GT/s */
#define	PCI_EXP_LNKCAP2_SLS_8_0GB 0x08	/* Supported Link Speed 8.0GT/s */
#define	PCI_EXP_LNKCAP2_SLS_16_0GB 0x10	/* Supported Link Speed 16.0GT/s */
#define	PCI_EXP_LNKCTL2_TLS		0x000f
#define	PCI_EXP_LNKCTL2_TLS_2_5GT	0x0001	/* Supported Speed 2.5GT/s */
#define	PCI_EXP_LNKCTL2_TLS_5_0GT	0x0002	/* Supported Speed 5GT/s */
#define	PCI_EXP_LNKCTL2_TLS_8_0GT	0x0003	/* Supported Speed 8GT/s */
#define	PCI_EXP_LNKCTL2_TLS_16_0GT	0x0004	/* Supported Speed 16GT/s */
#define	PCI_EXP_LNKCTL2_TLS_32_0GT	0x0005	/* Supported Speed 32GT/s */
#define	PCI_EXP_LNKCTL2_ENTER_COMP	0x0010	/* Enter Compliance */
#define	PCI_EXP_LNKCTL2_TX_MARGIN	0x0380	/* Transmit Margin */

#define PCI_EXP_LNKCAP_CLKPM	0x00040000
#define PCI_EXP_DEVSTA_TRPND	0x0020

#define	IORESOURCE_MEM	(1 << SYS_RES_MEMORY)
#define	IORESOURCE_IO	(1 << SYS_RES_IOPORT)
#define	IORESOURCE_IRQ	(1 << SYS_RES_IRQ)

enum pci_bus_speed {
	PCI_SPEED_UNKNOWN = -1,
	PCIE_SPEED_2_5GT,
	PCIE_SPEED_5_0GT,
	PCIE_SPEED_8_0GT,
	PCIE_SPEED_16_0GT,
};

enum pcie_link_width {
	PCIE_LNK_WIDTH_RESRV	= 0x00,
	PCIE_LNK_X1		= 0x01,
	PCIE_LNK_X2		= 0x02,
	PCIE_LNK_X4		= 0x04,
	PCIE_LNK_X8		= 0x08,
	PCIE_LNK_X12		= 0x0c,
	PCIE_LNK_X16		= 0x10,
	PCIE_LNK_X32		= 0x20,
	PCIE_LNK_WIDTH_UNKNOWN	= 0xff,
};

#define	PCIE_LINK_STATE_L0S		0x00000001
#define	PCIE_LINK_STATE_L1		0x00000002
#define	PCIE_LINK_STATE_CLKPM		0x00000004

typedef int pci_power_t;

#define PCI_D0	PCI_POWERSTATE_D0
#define PCI_D1	PCI_POWERSTATE_D1
#define PCI_D2	PCI_POWERSTATE_D2
#define PCI_D3hot	PCI_POWERSTATE_D3
#define PCI_D3cold	4

#define PCI_POWER_ERROR	PCI_POWERSTATE_UNKNOWN

#define	PCI_ERR_ROOT_COMMAND		PCIR_AER_ROOTERR_CMD
#define	PCI_ERR_ROOT_ERR_SRC		PCIR_AER_COR_SOURCE_ID

#define	PCI_EXT_CAP_ID_ERR		PCIZ_AER
#define	PCI_EXT_CAP_ID_L1SS		PCIZ_L1PM

#define	PCI_L1SS_CTL1			0x8
#define	PCI_L1SS_CTL1_L1SS_MASK		0xf

#define	PCI_IRQ_LEGACY			0x01
#define	PCI_IRQ_MSI			0x02
#define	PCI_IRQ_MSIX			0x04

struct pci_dev;

struct pci_driver {
	struct list_head		node;
	char				*name;
	const struct pci_device_id		*id_table;
	int  (*probe)(struct pci_dev *dev, const struct pci_device_id *id);
	void (*remove)(struct pci_dev *dev);
	int  (*suspend) (struct pci_dev *dev, pm_message_t state);	/* Device suspended */
	int  (*resume) (struct pci_dev *dev);		/* Device woken up */
	void (*shutdown) (struct pci_dev *dev);		/* Device shutdown */
	driver_t			bsddriver;
	devclass_t			bsdclass;
	struct device_driver		driver;
	const struct pci_error_handlers       *err_handler;
	bool				isdrm;
	int  (*bsd_iov_init)(device_t dev, uint16_t num_vfs,
	    const nvlist_t *pf_config);
	void  (*bsd_iov_uninit)(device_t dev);
	int  (*bsd_iov_add_vf)(device_t dev, uint16_t vfnum,
	    const nvlist_t *vf_config);
};

struct pci_bus {
	struct pci_dev	*self;
	int		domain;
	int		number;
};

extern struct list_head pci_drivers;
extern struct list_head pci_devices;
extern spinlock_t pci_lock;

#define	__devexit_p(x)	x

#define module_pci_driver(_driver)					\
									\
static inline int							\
_pci_init(void)								\
{									\
									\
	return (linux_pci_register_driver(&_driver));			\
}									\
									\
static inline void							\
_pci_exit(void)								\
{									\
									\
	linux_pci_unregister_driver(&_driver);				\
}									\
									\
module_init(_pci_init);							\
module_exit(_pci_exit)

/*
 * If we find drivers accessing this from multiple KPIs we may have to
 * refcount objects of this structure.
 */
struct pci_mmio_region {
	TAILQ_ENTRY(pci_mmio_region)	next;
	struct resource			*res;
	int				rid;
	int				type;
};

struct pci_dev {
	struct device		dev;
	struct list_head	links;
	struct pci_driver	*pdrv;
	struct pci_bus		*bus;
	struct pci_dev		*root;
	uint16_t		device;
	uint16_t		vendor;
	uint16_t		subsystem_vendor;
	uint16_t		subsystem_device;
	unsigned int		irq;
	unsigned int		devfn;
	uint32_t		class;
	uint8_t			revision;
	bool			managed;	/* devres "pcim_*(). */
	bool			want_iomap_res;
	bool			msi_enabled;
	bool			msix_enabled;
	phys_addr_t		rom;
	size_t			romlen;

	TAILQ_HEAD(, pci_mmio_region)	mmio;
};

/* We need some meta-struct to keep track of these for devres. */
struct pci_devres {
	bool		enable_io;
	/* PCIR_MAX_BAR_0 + 1 = 6 => BIT(0..5). */
	uint8_t		region_mask;
	struct resource	*region_table[PCIR_MAX_BAR_0 + 1]; /* Not needed. */
};
struct pcim_iomap_devres {
	void		*mmio_table[PCIR_MAX_BAR_0 + 1];
	struct resource	*res_table[PCIR_MAX_BAR_0 + 1];
};

/* Internal helper function(s). */
struct pci_dev *lkpinew_pci_dev(device_t);
void lkpi_pci_devres_release(struct device *, void *);
void lkpi_pcim_iomap_table_release(struct device *, void *);

static inline int
pci_resource_type(struct pci_dev *pdev, int bar)
{
	struct pci_map *pm;

	pm = pci_find_bar(pdev->dev.bsddev, PCIR_BAR(bar));
	if (!pm)
		return (-1);

	if (PCI_BAR_IO(pm->pm_value))
		return (SYS_RES_IOPORT);
	else
		return (SYS_RES_MEMORY);
}

struct resource_list_entry *linux_pci_reserve_bar(struct pci_dev *pdev,
		    struct resource_list *rl, int type, int rid);

static inline struct resource_list_entry *
linux_pci_get_rle(struct pci_dev *pdev, int type, int rid, bool reserve_bar)
{
	struct pci_devinfo *dinfo;
	struct resource_list *rl;
	struct resource_list_entry *rle;

	dinfo = device_get_ivars(pdev->dev.bsddev);
	rl = &dinfo->resources;
	rle = resource_list_find(rl, type, rid);
	/* Reserve resources for this BAR if needed. */
	if (rle == NULL && reserve_bar)
		rle = linux_pci_reserve_bar(pdev, rl, type, rid);
	return (rle);
}

static inline struct resource_list_entry *
linux_pci_get_bar(struct pci_dev *pdev, int bar, bool reserve)
{
	int type;

	type = pci_resource_type(pdev, bar);
	if (type < 0)
		return (NULL);
	bar = PCIR_BAR(bar);
	return (linux_pci_get_rle(pdev, type, bar, reserve));
}

static inline struct device *
linux_pci_find_irq_dev(unsigned int irq)
{
	struct pci_dev *pdev;
	struct device *found;

	found = NULL;
	spin_lock(&pci_lock);
	list_for_each_entry(pdev, &pci_devices, links) {
		if (irq == pdev->dev.irq ||
		    (irq >= pdev->dev.irq_start && irq < pdev->dev.irq_end)) {
			found = &pdev->dev;
			break;
		}
	}
	spin_unlock(&pci_lock);
	return (found);
}

/*
 * All drivers just seem to want to inspect the type not flags.
 */
static inline int
pci_resource_flags(struct pci_dev *pdev, int bar)
{
	int type;

	type = pci_resource_type(pdev, bar);
	if (type < 0)
		return (0);
	return (1 << type);
}

static inline const char *
pci_name(struct pci_dev *d)
{

	return device_get_desc(d->dev.bsddev);
}

static inline void *
pci_get_drvdata(struct pci_dev *pdev)
{

	return dev_get_drvdata(&pdev->dev);
}

static inline void
pci_set_drvdata(struct pci_dev *pdev, void *data)
{

	dev_set_drvdata(&pdev->dev, data);
}

static inline struct pci_dev *
pci_dev_get(struct pci_dev *pdev)
{

	if (pdev != NULL)
		get_device(&pdev->dev);
	return (pdev);
}

static __inline void
pci_dev_put(struct pci_dev *pdev)
{

	if (pdev != NULL)
		put_device(&pdev->dev);
}

static inline int
pci_enable_device(struct pci_dev *pdev)
{

	pci_enable_io(pdev->dev.bsddev, SYS_RES_IOPORT);
	pci_enable_io(pdev->dev.bsddev, SYS_RES_MEMORY);
	return (0);
}

static inline void
pci_disable_device(struct pci_dev *pdev)
{

	pci_disable_busmaster(pdev->dev.bsddev);
}

static inline int
pci_set_master(struct pci_dev *pdev)
{

	pci_enable_busmaster(pdev->dev.bsddev);
	return (0);
}

static inline int
pci_set_power_state(struct pci_dev *pdev, int state)
{

	pci_set_powerstate(pdev->dev.bsddev, state);
	return (0);
}

static inline int
pci_clear_master(struct pci_dev *pdev)
{

	pci_disable_busmaster(pdev->dev.bsddev);
	return (0);
}

static inline struct pci_devres *
lkpi_pci_devres_get_alloc(struct pci_dev *pdev)
{
	struct pci_devres *dr;

	dr = lkpi_devres_find(&pdev->dev, lkpi_pci_devres_release, NULL, NULL);
	if (dr == NULL) {
		dr = lkpi_devres_alloc(lkpi_pci_devres_release, sizeof(*dr),
		    GFP_KERNEL | __GFP_ZERO);
		if (dr != NULL)
			lkpi_devres_add(&pdev->dev, dr);
	}

	return (dr);
}
static inline struct pci_devres *
lkpi_pci_devres_find(struct pci_dev *pdev)
{

	if (!pdev->managed)
		return (NULL);

	return (lkpi_pci_devres_get_alloc(pdev));
}

static inline int
pci_request_region(struct pci_dev *pdev, int bar, const char *res_name)
{
	struct resource *res;
	struct pci_devres *dr;
	struct pci_mmio_region *mmio;
	int rid;
	int type;

	type = pci_resource_type(pdev, bar);
	if (type < 0)
		return (-ENODEV);
	rid = PCIR_BAR(bar);
	res = bus_alloc_resource_any(pdev->dev.bsddev, type, &rid,
	    RF_ACTIVE|RF_SHAREABLE);
	if (res == NULL) {
		device_printf(pdev->dev.bsddev, "%s: failed to alloc "
		    "bar %d type %d rid %d\n",
		    __func__, bar, type, PCIR_BAR(bar));
		return (-ENODEV);
	}

	/*
	 * It seems there is an implicit devres tracking on these if the device
	 * is managed; otherwise the resources are not automatiaclly freed on
	 * FreeBSD/LinuxKPI tough they should be/are expected to be by Linux
	 * drivers.
	 */
	dr = lkpi_pci_devres_find(pdev);
	if (dr != NULL) {
		dr->region_mask |= (1 << bar);
		dr->region_table[bar] = res;
	}

	/* Even if the device is not managed we need to track it for iomap. */
	mmio = malloc(sizeof(*mmio), M_DEVBUF, M_WAITOK | M_ZERO);
	mmio->rid = PCIR_BAR(bar);
	mmio->type = type;
	mmio->res = res;
	TAILQ_INSERT_TAIL(&pdev->mmio, mmio, next);

	return (0);
}

static inline void
pci_release_region(struct pci_dev *pdev, int bar)
{
	struct resource_list_entry *rle;
	struct pci_devres *dr;
	struct pci_mmio_region *mmio, *p;

	if ((rle = linux_pci_get_bar(pdev, bar, false)) == NULL)
		return;

	/*
	 * As we implicitly track the requests we also need to clear them on
	 * release.  Do clear before resource release.
	 */
	dr = lkpi_pci_devres_find(pdev);
	if (dr != NULL) {
		KASSERT(dr->region_table[bar] == rle->res, ("%s: pdev %p bar %d"
		    " region_table res %p != rel->res %p\n", __func__, pdev,
		    bar, dr->region_table[bar], rle->res));
		dr->region_table[bar] = NULL;
		dr->region_mask &= ~(1 << bar);
	}

	TAILQ_FOREACH_SAFE(mmio, &pdev->mmio, next, p) {
		if (rle->res != (void *)rman_get_bushandle(mmio->res))
			continue;
		TAILQ_REMOVE(&pdev->mmio, mmio, next);
		free(mmio, M_DEVBUF);
	}

	bus_release_resource(pdev->dev.bsddev, rle->type, rle->rid, rle->res);
}

static inline void
pci_release_regions(struct pci_dev *pdev)
{
	int i;

	for (i = 0; i <= PCIR_MAX_BAR_0; i++)
		pci_release_region(pdev, i);
}

static inline int
pci_request_regions(struct pci_dev *pdev, const char *res_name)
{
	int error;
	int i;

	for (i = 0; i <= PCIR_MAX_BAR_0; i++) {
		error = pci_request_region(pdev, i, res_name);
		if (error && error != -ENODEV) {
			pci_release_regions(pdev);
			return (error);
		}
	}
	return (0);
}

static inline void
lkpi_pci_disable_msix(struct pci_dev *pdev)
{

	pci_release_msi(pdev->dev.bsddev);

	/*
	 * The MSIX IRQ numbers associated with this PCI device are no
	 * longer valid and might be re-assigned. Make sure
	 * linux_pci_find_irq_dev() does no longer see them by
	 * resetting their references to zero:
	 */
	pdev->dev.irq_start = 0;
	pdev->dev.irq_end = 0;
	pdev->msix_enabled = false;
}
/* Only for consistency. No conflict on that one. */
#define	pci_disable_msix(pdev)		lkpi_pci_disable_msix(pdev)

static inline void
lkpi_pci_disable_msi(struct pci_dev *pdev)
{

	pci_release_msi(pdev->dev.bsddev);

	pdev->dev.irq_start = 0;
	pdev->dev.irq_end = 0;
	pdev->irq = pdev->dev.irq;
	pdev->msi_enabled = false;
}
#define	pci_disable_msi(pdev)		lkpi_pci_disable_msi(pdev)
#define	pci_free_irq_vectors(pdev)	lkpi_pci_disable_msi(pdev)

unsigned long	pci_resource_start(struct pci_dev *pdev, int bar);
unsigned long	pci_resource_len(struct pci_dev *pdev, int bar);

static inline bus_addr_t
pci_bus_address(struct pci_dev *pdev, int bar)
{

	return (pci_resource_start(pdev, bar));
}

#define	PCI_CAP_ID_EXP	PCIY_EXPRESS
#define	PCI_CAP_ID_PCIX	PCIY_PCIX
#define PCI_CAP_ID_AGP  PCIY_AGP
#define PCI_CAP_ID_PM   PCIY_PMG

#define PCI_EXP_DEVCTL		PCIER_DEVICE_CTL
#define PCI_EXP_DEVCTL_PAYLOAD	PCIEM_CTL_MAX_PAYLOAD
#define PCI_EXP_DEVCTL_READRQ	PCIEM_CTL_MAX_READ_REQUEST
#define PCI_EXP_LNKCTL		PCIER_LINK_CTL
#define PCI_EXP_LNKSTA		PCIER_LINK_STA

static inline int
pci_find_capability(struct pci_dev *pdev, int capid)
{
	int reg;

	if (pci_find_cap(pdev->dev.bsddev, capid, &reg))
		return (0);
	return (reg);
}

static inline int pci_pcie_cap(struct pci_dev *dev)
{
	return pci_find_capability(dev, PCI_CAP_ID_EXP);
}

static inline int
pci_find_ext_capability(struct pci_dev *pdev, int capid)
{
	int reg;

	if (pci_find_extcap(pdev->dev.bsddev, capid, &reg))
		return (0);
	return (reg);
}

#define	PCIM_PCAP_PME_SHIFT	11
static __inline bool
pci_pme_capable(struct pci_dev *pdev, uint32_t flag)
{
	struct pci_devinfo *dinfo;
	pcicfgregs *cfg;

	if (flag > (PCIM_PCAP_D3PME_COLD >> PCIM_PCAP_PME_SHIFT))
		return (false);

	dinfo = device_get_ivars(pdev->dev.bsddev);
	cfg = &dinfo->cfg;

	if (cfg->pp.pp_cap == 0)
		return (false);

	if ((cfg->pp.pp_cap & (1 << (PCIM_PCAP_PME_SHIFT + flag))) != 0)
		return (true);

	return (false);
}

static inline int
pci_disable_link_state(struct pci_dev *pdev, uint32_t flags)
{

	if (!pci_enable_aspm)
		return (-EPERM);

	return (-ENXIO);
}

static inline int
pci_read_config_byte(const struct pci_dev *pdev, int where, u8 *val)
{

	*val = (u8)pci_read_config(pdev->dev.bsddev, where, 1);
	return (0);
}

static inline int
pci_read_config_word(const struct pci_dev *pdev, int where, u16 *val)
{

	*val = (u16)pci_read_config(pdev->dev.bsddev, where, 2);
	return (0);
}

static inline int
pci_read_config_dword(const struct pci_dev *pdev, int where, u32 *val)
{

	*val = (u32)pci_read_config(pdev->dev.bsddev, where, 4);
	return (0);
}

static inline int
pci_write_config_byte(const struct pci_dev *pdev, int where, u8 val)
{

	pci_write_config(pdev->dev.bsddev, where, val, 1);
	return (0);
}

static inline int
pci_write_config_word(const struct pci_dev *pdev, int where, u16 val)
{

	pci_write_config(pdev->dev.bsddev, where, val, 2);
	return (0);
}

static inline int
pci_write_config_dword(const struct pci_dev *pdev, int where, u32 val)
{

	pci_write_config(pdev->dev.bsddev, where, val, 4);
	return (0);
}

int	linux_pci_register_driver(struct pci_driver *pdrv);
int	linux_pci_register_drm_driver(struct pci_driver *pdrv);
void	linux_pci_unregister_driver(struct pci_driver *pdrv);
void	linux_pci_unregister_drm_driver(struct pci_driver *pdrv);

#define	pci_register_driver(pdrv)	linux_pci_register_driver(pdrv)
#define	pci_unregister_driver(pdrv)	linux_pci_unregister_driver(pdrv)

struct msix_entry {
	int entry;
	int vector;
};

/*
 * Enable msix, positive errors indicate actual number of available
 * vectors.  Negative errors are failures.
 *
 * NB: define added to prevent this definition of pci_enable_msix from
 * clashing with the native FreeBSD version.
 */
#define	pci_enable_msix(...) \
  linux_pci_enable_msix(__VA_ARGS__)

static inline int
pci_enable_msix(struct pci_dev *pdev, struct msix_entry *entries, int nreq)
{
	struct resource_list_entry *rle;
	int error;
	int avail;
	int i;

	avail = pci_msix_count(pdev->dev.bsddev);
	if (avail < nreq) {
		if (avail == 0)
			return -EINVAL;
		return avail;
	}
	avail = nreq;
	if ((error = -pci_alloc_msix(pdev->dev.bsddev, &avail)) != 0)
		return error;
	/*
	 * Handle case where "pci_alloc_msix()" may allocate less
	 * interrupts than available and return with no error:
	 */
	if (avail < nreq) {
		pci_release_msi(pdev->dev.bsddev);
		return avail;
	}
	rle = linux_pci_get_rle(pdev, SYS_RES_IRQ, 1, false);
	pdev->dev.irq_start = rle->start;
	pdev->dev.irq_end = rle->start + avail;
	for (i = 0; i < nreq; i++)
		entries[i].vector = pdev->dev.irq_start + i;
	pdev->msix_enabled = true;
	return (0);
}

#define	pci_enable_msix_range(...) \
  linux_pci_enable_msix_range(__VA_ARGS__)

static inline int
pci_enable_msix_range(struct pci_dev *dev, struct msix_entry *entries,
    int minvec, int maxvec)
{
	int nvec = maxvec;
	int rc;

	if (maxvec < minvec)
		return (-ERANGE);

	do {
		rc = pci_enable_msix(dev, entries, nvec);
		if (rc < 0) {
			return (rc);
		} else if (rc > 0) {
			if (rc < minvec)
				return (-ENOSPC);
			nvec = rc;
		}
	} while (rc);
	return (nvec);
}

#define	pci_enable_msi(pdev) \
  linux_pci_enable_msi(pdev)

static inline int
pci_enable_msi(struct pci_dev *pdev)
{
	struct resource_list_entry *rle;
	int error;
	int avail;

	avail = pci_msi_count(pdev->dev.bsddev);
	if (avail < 1)
		return -EINVAL;

	avail = 1;	/* this function only enable one MSI IRQ */
	if ((error = -pci_alloc_msi(pdev->dev.bsddev, &avail)) != 0)
		return error;

	rle = linux_pci_get_rle(pdev, SYS_RES_IRQ, 1, false);
	pdev->dev.irq_start = rle->start;
	pdev->dev.irq_end = rle->start + avail;
	pdev->irq = rle->start;
	pdev->msi_enabled = true;
	return (0);
}

static inline int
pci_alloc_irq_vectors(struct pci_dev *pdev, int minv, int maxv,
    unsigned int flags)
{
	int error;

	if (flags & PCI_IRQ_MSIX) {
		struct msix_entry *entries;
		int i;

		entries = kcalloc(maxv, sizeof(*entries), GFP_KERNEL);
		if (entries == NULL) {
			error = -ENOMEM;
			goto out;
		}
		for (i = 0; i < maxv; ++i)
			entries[i].entry = i;
		error = pci_enable_msix(pdev, entries, maxv);
out:
		kfree(entries);
		if (error == 0 && pdev->msix_enabled)
			return (pdev->dev.irq_end - pdev->dev.irq_start);
	}
	if (flags & PCI_IRQ_MSI) {
		error = pci_enable_msi(pdev);
		if (error == 0 && pdev->msi_enabled)
			return (pdev->dev.irq_end - pdev->dev.irq_start);
	}
	if (flags & PCI_IRQ_LEGACY) {
		if (pdev->irq)
			return (1);
	}

	return (-EINVAL);
}

static inline int
pci_channel_offline(struct pci_dev *pdev)
{

	return (pci_read_config(pdev->dev.bsddev, PCIR_VENDOR, 2) == PCIV_INVALID);
}

static inline int pci_enable_sriov(struct pci_dev *dev, int nr_virtfn)
{
	return -ENODEV;
}
static inline void pci_disable_sriov(struct pci_dev *dev)
{
}

static inline struct resource *
_lkpi_pci_iomap(struct pci_dev *pdev, int bar, int mmio_size __unused)
{
	struct pci_mmio_region *mmio, *p;
	int type;

	type = pci_resource_type(pdev, bar);
	if (type < 0) {
		device_printf(pdev->dev.bsddev, "%s: bar %d type %d\n",
		     __func__, bar, type);
		return (NULL);
	}

	/*
	 * Check for duplicate mappings.
	 * This can happen if a driver calls pci_request_region() first.
	 */
	TAILQ_FOREACH_SAFE(mmio, &pdev->mmio, next, p) {
		if (mmio->type == type && mmio->rid == PCIR_BAR(bar)) {
			return (mmio->res);
		}
	}

	mmio = malloc(sizeof(*mmio), M_DEVBUF, M_WAITOK | M_ZERO);
	mmio->rid = PCIR_BAR(bar);
	mmio->type = type;
	mmio->res = bus_alloc_resource_any(pdev->dev.bsddev, mmio->type,
	    &mmio->rid, RF_ACTIVE|RF_SHAREABLE);
	if (mmio->res == NULL) {
		device_printf(pdev->dev.bsddev, "%s: failed to alloc "
		    "bar %d type %d rid %d\n",
		    __func__, bar, type, PCIR_BAR(bar));
		free(mmio, M_DEVBUF);
		return (NULL);
	}
	TAILQ_INSERT_TAIL(&pdev->mmio, mmio, next);

	return (mmio->res);
}

static inline void *
pci_iomap(struct pci_dev *pdev, int mmio_bar, int mmio_size)
{
	struct resource *res;

	res = _lkpi_pci_iomap(pdev, mmio_bar, mmio_size);
	if (res == NULL)
		return (NULL);
	/* This is a FreeBSD extension so we can use bus_*(). */
	if (pdev->want_iomap_res)
		return (res);
	return ((void *)rman_get_bushandle(res));
}

static inline void
pci_iounmap(struct pci_dev *pdev, void *res)
{
	struct pci_mmio_region *mmio, *p;

	TAILQ_FOREACH_SAFE(mmio, &pdev->mmio, next, p) {
		if (res != (void *)rman_get_bushandle(mmio->res))
			continue;
		bus_release_resource(pdev->dev.bsddev,
		    mmio->type, mmio->rid, mmio->res);
		TAILQ_REMOVE(&pdev->mmio, mmio, next);
		free(mmio, M_DEVBUF);
		return;
	}
}

static inline void
lkpi_pci_save_state(struct pci_dev *pdev)
{

	pci_save_state(pdev->dev.bsddev);
}

static inline void
lkpi_pci_restore_state(struct pci_dev *pdev)
{

	pci_restore_state(pdev->dev.bsddev);
}

#define pci_save_state(dev)	lkpi_pci_save_state(dev)
#define pci_restore_state(dev)	lkpi_pci_restore_state(dev)

#define DEFINE_PCI_DEVICE_TABLE(_table) \
	const struct pci_device_id _table[] __devinitdata

/* XXX This should not be necessary. */
#define	pcix_set_mmrbc(d, v)	0
#define	pcix_get_max_mmrbc(d)	0
#define	pcie_set_readrq(d, v)	pci_set_max_read_req((d)->dev.bsddev, (v))

#define	PCI_DMA_BIDIRECTIONAL	0
#define	PCI_DMA_TODEVICE	1
#define	PCI_DMA_FROMDEVICE	2
#define	PCI_DMA_NONE		3

#define	pci_pool		dma_pool
#define	pci_pool_destroy(...)	dma_pool_destroy(__VA_ARGS__)
#define	pci_pool_alloc(...)	dma_pool_alloc(__VA_ARGS__)
#define	pci_pool_free(...)	dma_pool_free(__VA_ARGS__)
#define	pci_pool_create(_name, _pdev, _size, _align, _alloc)		\
	    dma_pool_create(_name, &(_pdev)->dev, _size, _align, _alloc)
#define	pci_free_consistent(_hwdev, _size, _vaddr, _dma_handle)		\
	    dma_free_coherent((_hwdev) == NULL ? NULL : &(_hwdev)->dev,	\
		_size, _vaddr, _dma_handle)
#define	pci_map_sg(_hwdev, _sg, _nents, _dir)				\
	    dma_map_sg((_hwdev) == NULL ? NULL : &(_hwdev->dev),	\
		_sg, _nents, (enum dma_data_direction)_dir)
#define	pci_map_single(_hwdev, _ptr, _size, _dir)			\
	    dma_map_single((_hwdev) == NULL ? NULL : &(_hwdev->dev),	\
		(_ptr), (_size), (enum dma_data_direction)_dir)
#define	pci_unmap_single(_hwdev, _addr, _size, _dir)			\
	    dma_unmap_single((_hwdev) == NULL ? NULL : &(_hwdev)->dev,	\
		_addr, _size, (enum dma_data_direction)_dir)
#define	pci_unmap_sg(_hwdev, _sg, _nents, _dir)				\
	    dma_unmap_sg((_hwdev) == NULL ? NULL : &(_hwdev)->dev,	\
		_sg, _nents, (enum dma_data_direction)_dir)
#define	pci_map_page(_hwdev, _page, _offset, _size, _dir)		\
	    dma_map_page((_hwdev) == NULL ? NULL : &(_hwdev)->dev, _page,\
		_offset, _size, (enum dma_data_direction)_dir)
#define	pci_unmap_page(_hwdev, _dma_address, _size, _dir)		\
	    dma_unmap_page((_hwdev) == NULL ? NULL : &(_hwdev)->dev,	\
		_dma_address, _size, (enum dma_data_direction)_dir)
#define	pci_set_dma_mask(_pdev, mask)	dma_set_mask(&(_pdev)->dev, (mask))
#define	pci_dma_mapping_error(_pdev, _dma_addr)				\
	    dma_mapping_error(&(_pdev)->dev, _dma_addr)
#define	pci_set_consistent_dma_mask(_pdev, _mask)			\
	    dma_set_coherent_mask(&(_pdev)->dev, (_mask))
#define	DECLARE_PCI_UNMAP_ADDR(x)	DEFINE_DMA_UNMAP_ADDR(x);
#define	DECLARE_PCI_UNMAP_LEN(x)	DEFINE_DMA_UNMAP_LEN(x);
#define	pci_unmap_addr		dma_unmap_addr
#define	pci_unmap_addr_set	dma_unmap_addr_set
#define	pci_unmap_len		dma_unmap_len
#define	pci_unmap_len_set	dma_unmap_len_set

typedef unsigned int __bitwise pci_channel_state_t;
typedef unsigned int __bitwise pci_ers_result_t;

enum pci_channel_state {
	pci_channel_io_normal = 1,
	pci_channel_io_frozen = 2,
	pci_channel_io_perm_failure = 3,
};

enum pci_ers_result {
	PCI_ERS_RESULT_NONE = 1,
	PCI_ERS_RESULT_CAN_RECOVER = 2,
	PCI_ERS_RESULT_NEED_RESET = 3,
	PCI_ERS_RESULT_DISCONNECT = 4,
	PCI_ERS_RESULT_RECOVERED = 5,
};

/* PCI bus error event callbacks */
struct pci_error_handlers {
	pci_ers_result_t (*error_detected)(struct pci_dev *dev,
	    enum pci_channel_state error);
	pci_ers_result_t (*mmio_enabled)(struct pci_dev *dev);
	pci_ers_result_t (*link_reset)(struct pci_dev *dev);
	pci_ers_result_t (*slot_reset)(struct pci_dev *dev);
	void (*resume)(struct pci_dev *dev);
};

/* FreeBSD does not support SRIOV - yet */
static inline struct pci_dev *pci_physfn(struct pci_dev *dev)
{
	return dev;
}

static inline bool pci_is_pcie(struct pci_dev *dev)
{
	return !!pci_pcie_cap(dev);
}

static inline u16 pcie_flags_reg(struct pci_dev *dev)
{
	int pos;
	u16 reg16;

	pos = pci_find_capability(dev, PCI_CAP_ID_EXP);
	if (!pos)
		return 0;

	pci_read_config_word(dev, pos + PCI_EXP_FLAGS, &reg16);

	return reg16;
}

static inline int pci_pcie_type(struct pci_dev *dev)
{
	return (pcie_flags_reg(dev) & PCI_EXP_FLAGS_TYPE) >> 4;
}

static inline int pcie_cap_version(struct pci_dev *dev)
{
	return pcie_flags_reg(dev) & PCI_EXP_FLAGS_VERS;
}

static inline bool pcie_cap_has_lnkctl(struct pci_dev *dev)
{
	int type = pci_pcie_type(dev);

	return pcie_cap_version(dev) > 1 ||
	       type == PCI_EXP_TYPE_ROOT_PORT ||
	       type == PCI_EXP_TYPE_ENDPOINT ||
	       type == PCI_EXP_TYPE_LEG_END;
}

static inline bool pcie_cap_has_devctl(const struct pci_dev *dev)
{
		return true;
}

static inline bool pcie_cap_has_sltctl(struct pci_dev *dev)
{
	int type = pci_pcie_type(dev);

	return pcie_cap_version(dev) > 1 || type == PCI_EXP_TYPE_ROOT_PORT ||
	    (type == PCI_EXP_TYPE_DOWNSTREAM &&
	    pcie_flags_reg(dev) & PCI_EXP_FLAGS_SLOT);
}

static inline bool pcie_cap_has_rtctl(struct pci_dev *dev)
{
	int type = pci_pcie_type(dev);

	return pcie_cap_version(dev) > 1 || type == PCI_EXP_TYPE_ROOT_PORT ||
	    type == PCI_EXP_TYPE_RC_EC;
}

static bool pcie_capability_reg_implemented(struct pci_dev *dev, int pos)
{
	if (!pci_is_pcie(dev))
		return false;

	switch (pos) {
	case PCI_EXP_FLAGS_TYPE:
		return true;
	case PCI_EXP_DEVCAP:
	case PCI_EXP_DEVCTL:
	case PCI_EXP_DEVSTA:
		return pcie_cap_has_devctl(dev);
	case PCI_EXP_LNKCAP:
	case PCI_EXP_LNKCTL:
	case PCI_EXP_LNKSTA:
		return pcie_cap_has_lnkctl(dev);
	case PCI_EXP_SLTCAP:
	case PCI_EXP_SLTCTL:
	case PCI_EXP_SLTSTA:
		return pcie_cap_has_sltctl(dev);
	case PCI_EXP_RTCTL:
	case PCI_EXP_RTCAP:
	case PCI_EXP_RTSTA:
		return pcie_cap_has_rtctl(dev);
	case PCI_EXP_DEVCAP2:
	case PCI_EXP_DEVCTL2:
	case PCI_EXP_LNKCAP2:
	case PCI_EXP_LNKCTL2:
	case PCI_EXP_LNKSTA2:
		return pcie_cap_version(dev) > 1;
	default:
		return false;
	}
}

static inline int
pcie_capability_read_dword(struct pci_dev *dev, int pos, u32 *dst)
{
	if (pos & 3)
		return -EINVAL;

	if (!pcie_capability_reg_implemented(dev, pos))
		return -EINVAL;

	return pci_read_config_dword(dev, pci_pcie_cap(dev) + pos, dst);
}

static inline int
pcie_capability_read_word(struct pci_dev *dev, int pos, u16 *dst)
{
	if (pos & 3)
		return -EINVAL;

	if (!pcie_capability_reg_implemented(dev, pos))
		return -EINVAL;

	return pci_read_config_word(dev, pci_pcie_cap(dev) + pos, dst);
}

static inline int
pcie_capability_write_word(struct pci_dev *dev, int pos, u16 val)
{
	if (pos & 1)
		return -EINVAL;

	if (!pcie_capability_reg_implemented(dev, pos))
		return 0;

	return pci_write_config_word(dev, pci_pcie_cap(dev) + pos, val);
}

static inline int pcie_get_minimum_link(struct pci_dev *dev,
    enum pci_bus_speed *speed, enum pcie_link_width *width)
{
	*speed = PCI_SPEED_UNKNOWN;
	*width = PCIE_LNK_WIDTH_UNKNOWN;
	return (0);
}

static inline int
pci_num_vf(struct pci_dev *dev)
{
	return (0);
}

static inline enum pci_bus_speed
pcie_get_speed_cap(struct pci_dev *dev)
{
	device_t root;
	uint32_t lnkcap, lnkcap2;
	int error, pos;

	root = device_get_parent(dev->dev.bsddev);
	if (root == NULL)
		return (PCI_SPEED_UNKNOWN);
	root = device_get_parent(root);
	if (root == NULL)
		return (PCI_SPEED_UNKNOWN);
	root = device_get_parent(root);
	if (root == NULL)
		return (PCI_SPEED_UNKNOWN);

	if (pci_get_vendor(root) == PCI_VENDOR_ID_VIA ||
	    pci_get_vendor(root) == PCI_VENDOR_ID_SERVERWORKS)
		return (PCI_SPEED_UNKNOWN);

	if ((error = pci_find_cap(root, PCIY_EXPRESS, &pos)) != 0)
		return (PCI_SPEED_UNKNOWN);

	lnkcap2 = pci_read_config(root, pos + PCIER_LINK_CAP2, 4);

	if (lnkcap2) {	/* PCIe r3.0-compliant */
		if (lnkcap2 & PCI_EXP_LNKCAP2_SLS_2_5GB)
			return (PCIE_SPEED_2_5GT);
		if (lnkcap2 & PCI_EXP_LNKCAP2_SLS_5_0GB)
			return (PCIE_SPEED_5_0GT);
		if (lnkcap2 & PCI_EXP_LNKCAP2_SLS_8_0GB)
			return (PCIE_SPEED_8_0GT);
		if (lnkcap2 & PCI_EXP_LNKCAP2_SLS_16_0GB)
			return (PCIE_SPEED_16_0GT);
	} else {	/* pre-r3.0 */
		lnkcap = pci_read_config(root, pos + PCIER_LINK_CAP, 4);
		if (lnkcap & PCI_EXP_LNKCAP_SLS_2_5GB)
			return (PCIE_SPEED_2_5GT);
		if (lnkcap & PCI_EXP_LNKCAP_SLS_5_0GB)
			return (PCIE_SPEED_5_0GT);
		if (lnkcap & PCI_EXP_LNKCAP_SLS_8_0GB)
			return (PCIE_SPEED_8_0GT);
		if (lnkcap & PCI_EXP_LNKCAP_SLS_16_0GB)
			return (PCIE_SPEED_16_0GT);
	}
	return (PCI_SPEED_UNKNOWN);
}

static inline enum pcie_link_width
pcie_get_width_cap(struct pci_dev *dev)
{
	uint32_t lnkcap;

	pcie_capability_read_dword(dev, PCI_EXP_LNKCAP, &lnkcap);
	if (lnkcap)
		return ((lnkcap & PCI_EXP_LNKCAP_MLW) >> 4);

	return (PCIE_LNK_WIDTH_UNKNOWN);
}

static inline int
pcie_get_mps(struct pci_dev *dev)
{
	return (pci_get_max_payload(dev->dev.bsddev));
}

static inline uint32_t
PCIE_SPEED2MBS_ENC(enum pci_bus_speed spd)
{

	switch(spd) {
	case PCIE_SPEED_16_0GT:
		return (16000 * 128 / 130);
	case PCIE_SPEED_8_0GT:
		return (8000 * 128 / 130);
	case PCIE_SPEED_5_0GT:
		return (5000 * 8 / 10);
	case PCIE_SPEED_2_5GT:
		return (2500 * 8 / 10);
	default:
		return (0);
	}
}

static inline uint32_t
pcie_bandwidth_available(struct pci_dev *pdev,
    struct pci_dev **limiting,
    enum pci_bus_speed *speed,
    enum pcie_link_width *width)
{
	enum pci_bus_speed nspeed = pcie_get_speed_cap(pdev);
	enum pcie_link_width nwidth = pcie_get_width_cap(pdev);

	if (speed)
		*speed = nspeed;
	if (width)
		*width = nwidth;

	return (nwidth * PCIE_SPEED2MBS_ENC(nspeed));
}

static inline struct pci_dev *
pcie_find_root_port(struct pci_dev *pdev)
{
	device_t root;

	if (pdev->root != NULL)
		return (pdev->root);

	root = pci_find_pcie_root_port(pdev->dev.bsddev);
	if (root == NULL)
		return (NULL);

	pdev->root = lkpinew_pci_dev(root);
	return (pdev->root);
}

/* This is needed when people rip out the device "HotPlug". */
static inline void
pci_lock_rescan_remove(void)
{
}

static inline void
pci_unlock_rescan_remove(void)
{
}

static __inline void
pci_stop_and_remove_bus_device(struct pci_dev *pdev)
{
}

/*
 * The following functions can be used to attach/detach the LinuxKPI's
 * PCI device runtime. The pci_driver and pci_device_id pointer is
 * allowed to be NULL. Other pointers must be all valid.
 * The pci_dev structure should be zero-initialized before passed
 * to the linux_pci_attach_device function.
 */
extern int linux_pci_attach_device(device_t, struct pci_driver *,
    const struct pci_device_id *, struct pci_dev *);
extern int linux_pci_detach_device(struct pci_dev *);

static inline int
pci_dev_present(const struct pci_device_id *cur)
{
	while (cur != NULL && (cur->vendor || cur->device)) {
		if (pci_find_device(cur->vendor, cur->device) != NULL) {
			return (1);
		}
		cur++;
	}
	return (0);
}

static inline bool
pci_is_root_bus(struct pci_bus *pbus)
{

	return (pbus->self == NULL);
}

struct pci_dev *lkpi_pci_get_domain_bus_and_slot(int domain,
    unsigned int bus, unsigned int devfn);
#define	pci_get_domain_bus_and_slot(domain, bus, devfn)	\
	lkpi_pci_get_domain_bus_and_slot(domain, bus, devfn)

static inline int
pci_domain_nr(struct pci_bus *pbus)
{

	return (pbus->domain);
}

static inline int
pci_bus_read_config(struct pci_bus *bus, unsigned int devfn,
                    int pos, uint32_t *val, int len)
{

	*val = pci_read_config(bus->self->dev.bsddev, pos, len);
	return (0);
}

static inline int
pci_bus_read_config_word(struct pci_bus *bus, unsigned int devfn, int pos, u16 *val)
{
	uint32_t tmp;
	int ret;

	ret = pci_bus_read_config(bus, devfn, pos, &tmp, 2);
	*val = (u16)tmp;
	return (ret);
}

static inline int
pci_bus_read_config_byte(struct pci_bus *bus, unsigned int devfn, int pos, u8 *val)
{
	uint32_t tmp;
	int ret;

	ret = pci_bus_read_config(bus, devfn, pos, &tmp, 1);
	*val = (u8)tmp;
	return (ret);
}

static inline int
pci_bus_write_config(struct pci_bus *bus, unsigned int devfn, int pos,
    uint32_t val, int size)
{

	pci_write_config(bus->self->dev.bsddev, pos, val, size);
	return (0);
}

static inline int
pci_bus_write_config_byte(struct pci_bus *bus, unsigned int devfn, int pos,
    uint8_t val)
{
	return (pci_bus_write_config(bus, devfn, pos, val, 1));
}

static inline int
pci_bus_write_config_word(struct pci_bus *bus, unsigned int devfn, int pos,
    uint16_t val)
{
	return (pci_bus_write_config(bus, devfn, pos, val, 2));
}

struct pci_dev *lkpi_pci_get_class(unsigned int class, struct pci_dev *from);
#define	pci_get_class(class, from)	lkpi_pci_get_class(class, from)

/* -------------------------------------------------------------------------- */

static inline int
pcim_enable_device(struct pci_dev *pdev)
{
	struct pci_devres *dr;
	int error;

	/* Here we cannot run through the pdev->managed check. */
	dr = lkpi_pci_devres_get_alloc(pdev);
	if (dr == NULL)
		return (-ENOMEM);

	/* If resources were enabled before do not do it again. */
	if (dr->enable_io)
		return (0);

	error = pci_enable_device(pdev);
	if (error == 0)
		dr->enable_io = true;

	/* This device is not managed. */
	pdev->managed = true;

	return (error);
}

static inline struct pcim_iomap_devres *
lkpi_pcim_iomap_devres_find(struct pci_dev *pdev)
{
	struct pcim_iomap_devres *dr;

	dr = lkpi_devres_find(&pdev->dev, lkpi_pcim_iomap_table_release,
	    NULL, NULL);
	if (dr == NULL) {
		dr = lkpi_devres_alloc(lkpi_pcim_iomap_table_release,
		    sizeof(*dr), GFP_KERNEL | __GFP_ZERO);
		if (dr != NULL)
			lkpi_devres_add(&pdev->dev, dr);
	}

	if (dr == NULL)
		device_printf(pdev->dev.bsddev, "%s: NULL\n", __func__);

	return (dr);
}

static inline void __iomem **
pcim_iomap_table(struct pci_dev *pdev)
{
	struct pcim_iomap_devres *dr;

	dr = lkpi_pcim_iomap_devres_find(pdev);
	if (dr == NULL)
		return (NULL);

	/*
	 * If the driver has manually set a flag to be able to request the
	 * resource to use bus_read/write_<n>, return the shadow table.
	 */
	if (pdev->want_iomap_res)
		return ((void **)dr->res_table);

	/* This is the Linux default. */
	return (dr->mmio_table);
}

static inline int
pcim_iomap_regions_request_all(struct pci_dev *pdev, uint32_t mask, char *name)
{
	struct pcim_iomap_devres *dr;
	void *res;
	uint32_t mappings, requests, req_mask;
	int bar, error;

	dr = lkpi_pcim_iomap_devres_find(pdev);
	if (dr == NULL)
		return (-ENOMEM);

	/* Request all the BARs ("regions") we do not iomap. */
	req_mask = ((1 << (PCIR_MAX_BAR_0 + 1)) - 1) & ~mask;
	for (bar = requests = 0; requests != req_mask; bar++) {
		if ((req_mask & (1 << bar)) == 0)
			continue;
		error = pci_request_region(pdev, bar, name);
		if (error != 0 && error != -ENODEV)
			goto err;
		requests |= (1 << bar);
	}

	/* Now iomap all the requested (by "mask") ones. */
	for (bar = mappings = 0; mappings != mask; bar++) {
		if ((mask & (1 << bar)) == 0)
			continue;

		/* Request double is not allowed. */
		if (dr->mmio_table[bar] != NULL) {
			device_printf(pdev->dev.bsddev, "%s: bar %d %p\n",
			     __func__, bar, dr->mmio_table[bar]);
			goto err;
		}

		res = _lkpi_pci_iomap(pdev, bar, 0);
		if (res == NULL)
			goto err;
		dr->mmio_table[bar] = (void *)rman_get_bushandle(res);
		dr->res_table[bar] = res;

		mappings |= (1 << bar);
	}

	return (0);

err:
	for (bar = PCIR_MAX_BAR_0; bar >= 0; bar--) {
		if ((mappings & (1 << bar)) != 0) {
			res = dr->mmio_table[bar];
			if (res == NULL)
				continue;
			pci_iounmap(pdev, res);
		} else if ((requests & (1 << bar)) != 0) {
			pci_release_region(pdev, bar);
		}
	}

	return (-EINVAL);
}

/* This is a FreeBSD extension so we can use bus_*(). */
static inline void
linuxkpi_pcim_want_to_use_bus_functions(struct pci_dev *pdev)
{
	pdev->want_iomap_res = true;
}

#endif	/* _LINUXKPI_LINUX_PCI_H_ */
