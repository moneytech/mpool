/* SPDX-License-Identifier: MIT */
/*
 * Copyright (C) 2015-2020 Micron Technology, Inc.  All rights reserved.
 */
/*
 * Mpool Management APIs
 */

#ifndef MPOOL_H
#define MPOOL_H

#include <stdint.h>
#include <stdbool.h>
#include <uuid/uuid.h>
#include <sys/param.h>
typedef uuid_t uuid_le;

#include <mpool/mpool_ioctl.h>

typedef int64_t                 mpool_err_t;

struct mpool;                   /* opaque mpool handle */
struct mpool_mdc;               /* opaque MDC (metadata container) handle */
struct mpool_mcache_map;        /* opaque mcache map handle */
struct mpool_mlog;              /* opaque mlog handle */
struct iovec;

#define MPOOL_RUNDIR_ROOT       "/var/run/mpool"

/* MTF_MOCK_DECL(mpool) */

#define mp_errinfo              mpool_devrpt

/**
 * mpool_strerror() - Format errno description from merr_t
 */
char *mpool_strerror(mpool_err_t err, char *buf, size_t bufsz);

/**
 * mpool_strinfo() - Format file, line, and errno from merr_t
 */
char *mpool_strinfo(mpool_err_t err, char *buf, size_t bufsz);

int mpool_errno(mpool_err_t err);


/*
 * Mpool Administrative APIs...
 */

/**
 * mpool_create() - Create an mpool in the default media class
 * @mpname:  mpool name
 * @devname: device name
 * @params:  mpool params
 * @flags:   mpool management flags
 * @ei:      error detail
 */
uint64_t
mpool_create(
	const char             *mpname,
	const char             *devname,
	struct mpool_params    *params,
	uint32_t                flags,
	struct mpool_devrpt    *ei);

/**
 * mpool_destroy() - Deactivate and destroy an mpool
 * @mpname: mpool name
 * @flags:  mpool management flags
 * @ei:     error detail
 */
/* MTF_MOCK */
uint64_t
mpool_destroy(
	const char             *mpname,
	uint32_t                flags,
	struct mpool_devrpt    *ei);

/**
 * mpool_scan() - Scan disks for mpools
 * @propcp:   (output) count of mpools found
 * @propvp:   (output) vector of mpool properties
 * @ei:       error detail
 *
 * %mpool_scan scans all drives on the system and builds
 * a list of tuples (mpool name and uuid) for each mpool found.
 *
 * Note that only valid fields in each struct mpool_params record
 * are %mp_poolid and %mp_name.
 */
uint64_t
mpool_scan(
	int                    *propcp,
	struct mpool_params   **propvp,
	struct mpool_devrpt    *ei);

/**
 * mpool_list() - Get list of active mpools
 * @propcp:   (output) count of mpools found
 * @propvp:   (output) vector of mpool properties
 * @ei:       error detail
 *
 * %mpool_list retrieves the list of active mpools from the mpool
 * kernel module.
 */
uint64_t
mpool_list(
	int                    *propcp,
	struct mpool_params   **propvp,
	struct mpool_devrpt    *ei);

/**
 * mpool_activate() - Activate an mpool
 * @mpname: name of mpool to activate
 * @params: mpool parameters
 * @flags:  mpool management flags
 * @ei:     error detail
 *
 * Discovers associated block devices and attempts cohesive activate.
 */
uint64_t
mpool_activate(
	const char             *mpname,
	struct mpool_params    *params,
	uint32_t                flags,
	struct mpool_devrpt    *ei);

/**
 * mpool_deactivate() - Deactivate an mpool by name
 * @mpname: mpool name
 * @flags:  mpool management flags
 * @ei:     error detail
 */
uint64_t
mpool_deactivate(
	const char             *mpname,
	uint32_t                flags,
	struct mpool_devrpt    *ei);

/**
 * mpool_rename() - Rename an mpool by name
 * @oldmp: old mpool name
 * @newmp: new mpool name
 * @flags:  mpool management flags
 * @ei:     error detail
 */
uint64_t
mpool_rename(
	const char             *oldmp,
	const char             *newmp,
	uint32_t                flags,
	struct mpool_devrpt    *ei);

/**
 * mpool_open() - Open an mpool
 * @mp_name:  mpool name
 * @flags:    open flags
 * @ds:       mpool handle (output)
 * @ei:       error detail
 *
 * Flags are limited to a subset of flags allowed by open(2):
 * O_RDONLY, O_WRONLY, O_RDWR, and O_EXCL.
 *
 * If the O_EXCL flag is given on first open then all subsequent calls to
 * @mpool_open() will fail with -EBUSY.  Similarly, if the mpool is open in
 * shared mode then specifying the O_EXCL flag will fail with -EBUSY.
 */
/* MTF_MOCK */
uint64_t
mpool_open(
	const char             *mpname,
	uint32_t                flags,
	struct mpool          **mp,
	struct mpool_devrpt    *ei);

/**
 * mpool_close() - Close an mpool
 * @mp:       mpool handle
 */
/* MTF_MOCK */
uint64_t mpool_close(struct mpool *mp);

/**
 * mpool_ioctl() - wrapper around ioctl
 * @fd:      file descriptor
 * @cmd:     IOCTL command
 * @arg:     IOCTL-specific argument
 */
uint64_t mpool_ioctl(int fd, int cmd, void *arg);

/**
 * mpool_mclass_add() - Add a media class to an mpool
 * @mpname:  mpool name
 * @devname: device name
 * @mclass:  media class of the device
 * @flags:   mpool management flags
 * @ei:      error detail
 */
uint64_t
mpool_mclass_add(
	const char             *mpname,
	const char             *devname,
	enum mp_media_classp    mclass,
	struct mpool_params    *params,
	uint32_t                flags,
	struct mpool_devrpt    *ei);

/**
 * mpool_mclass_get() - get properties of the specified media class
 * @mp:      mpool descriptor
 * @mclass:  input media mclass
 * @props:   media class props (output)
 *
 * Returns: 0 for success and ENOENT if the specified mclass is not present
 */
/* MTF_MOCK */
uint64_t
mpool_mclass_get(
	struct mpool               *mp,
	enum mp_media_classp        mclass,
	struct mpool_mclass_props  *props);

/**
 * mpool_params_init() - initialize mpool params
 * @params: params instance to initialize
 */
void mpool_params_init(struct mpool_params *params);

/**
 * mpool_params_get() - get parameters of an activated mpool
 * @mp:  mpool handle
 * @params: mpool parameters
 * @ei:     error detail
 */
/* MTF_MOCK */
uint64_t
mpool_params_get(
	struct mpool           *mp,
	struct mpool_params    *params,
	struct mpool_devrpt    *ei);

/**
 * mpool_params_set() - set parameters of an activated mpool
 * @mp:  mpool handle
 * @params: mpool parameters
 * @ei:     error detail
 */
uint64_t
mpool_params_set(
	struct mpool           *mp,
	struct mpool_params    *params,
	struct mpool_devrpt    *ei);

/**
 * mpool_props_get() - Get properties of an mpool
 * @mp:  mpool handle
 * @props: mpool props (output)
 * @usage: mpool usage (output)
 */
uint64_t
mpool_props_get(
	struct mpool       *mp,
	struct mp_props    *props,
	struct mp_usage    *usage);

/**
 * mpool_devprops_get() - Get properties of a device within an mpool
 * @mp:  mpool handle
 * @devname: device name
 * @dprops: device props (output)
 */
uint64_t
mpool_devprops_get(
	struct mpool       *mp,
	const char         *devname,
	struct mp_devprops *dprops);

/*
 * Mpool Data Manager APIs
 */

/*
 * MLOG APIs
 */

/**
 * mpool_mlog_alloc() - allocate an mlog
 * @mp:         mpool handle
 * @capreq:     mlog capacity requirements
 * @mclassp:    mlog media class
 * @props:      properties of new mlog (output)
 * @mlh:        mlog handle (output)
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_alloc(
	struct mpool            *mp,
	struct mlog_capacity    *capreq,
	enum mp_media_classp     mclassp,
	struct mlog_props       *props,
	struct mpool_mlog      **mlh);

/**
 * mpool_mlog_realloc() - re-allocate an mlog
 * @mp:         mpool handle
 * @objid:      object ID
 * @capreq:     mlog capacity requirements
 * @mclassp:    mlog media class
 * @props:      properties of new mlog (output)
 * @mlh:        mlog handle (output)
 *
 * Return:
 *   %0 on success, <%0 on error
 */
uint64_t
mpool_mlog_realloc(
	struct mpool           *mp,
	uint64_t                objid,
	struct mlog_capacity   *capreq,
	enum mp_media_classp    mclassp,
	struct mlog_props       *props,
	struct mpool_mlog      **mlh);

/**
 * mpool_mlog_commit() - commit an mlog
 * @mp:        mpool handle
 * @mlh:       mlog handle
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_commit(
	struct mpool       *mp,
	struct mpool_mlog  *mlh);

/**
 * mpool_mlog_abort() - abort an mlog
 * @mp:        mpool handle
 * @mlh:       mlog handle
 *
 * Return:
 *   %0 on success, <%0 on error
 */
uint64_t
mpool_mlog_abort(
	struct mpool       *mp,
	struct mpool_mlog  *mlh);

/**
 * mpool_mlog_delete() - delete an mlog
 * @mp:        mpool handle
 * @mlh:       mlog handle
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_delete(
	struct mpool       *mp,
	struct mpool_mlog  *mlh);

/**
 * mpool_mlog_open() - open an mlog
 * @mp:        mpool handle
 * @mlh:       mlog handle
 * @flags:     Mlog open flags (enum mlog_open_flags)
 * @gen:       generation number (output)
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_open(
	struct mpool       *mp,
	struct mpool_mlog  *mlh,
	uint8_t             flags,
	uint64_t           *gen);

/**
 * mpool_mlog_close() - close an mlog
 * @mp:        mpool handle
 * @mlh:       mlog handle
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_close(
	struct mpool       *mp,
	struct mpool_mlog  *mlh);

/**
 * mpool_mlog_find_get() - returns a referenced mlog handle for the specified
 * object ID
 * @mp:        mpool handle
 * @objid:     object ID
 * @props:     mlog properties (output)
 * @mlh_out:   mlog handle (output)
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_find_get(
	struct mpool        *mp,
	uint64_t             objid,
	struct mlog_props   *props,
	struct mpool_mlog  **mlh_out);

/**
 * mpool_mlog_resolve() - resolves the specified object ID to a mlog handle
 * without taking reference.
 * @mp:        mpool handle
 * @objid:     object ID
 * @props:     mlog properties (output)
 * @mlh_out:   mlog handle (output)
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_resolve(
	struct mpool        *mp,
	uint64_t             objid,
	struct mlog_props   *props,
	struct mpool_mlog  **mlh_out);

/**
 * mpool_mlog_put() - puts the reference back on an mlog
 * mpool_mlog_find_get() and mpool_mlog_put() comes in pairs
 * @mp:        mpool handle
 * @mlh:       mlog handle
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_put(
	struct mpool        *mp,
	struct mpool_mlog   *mlh);

/**
 * mpool_mlog_append data() - Appenmp data to an mlog
 * @mp:        mpool handle
 * @mlh:       mlog handle
 * @data:      buffer to append data from
 * @len:       buffer len
 * @sync:      1 = sync; 0 = async append
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_append_data(
	struct mpool       *mp,
	struct mpool_mlog  *mlh,
	void               *data,
	size_t              len,
	int                 sync);

/**
 * mpool_mlog_append datav() - Appends data to an mlog
 * @mp:        mpool handle
 * @mlh:       mlog handle
 * @iov        buffer in the form of struct iovec
 * @len:       buffer len
 * @sync:      1 = sync; 0 = async append
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_append_datav(
	struct mpool       *mp,
	struct mpool_mlog  *mlh,
	struct iovec       *iov,
	size_t              len,
	int                 sync);

/**
 *mpool_mlog_read_data_init() - Initializes the internal read cursor to the
 *                              start of log
 * @mp:        mpool handle
 * @mlh:       mlog handle
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_read_data_init(
	struct mpool       *mp,
	struct mpool_mlog  *mlh);

/**
 * mpool_mlog_read_data_next() - Reads the next record from mlog based on what
 * the internal read cursor points to
 * @mp:        mpool handle
 * @mlh:       mlog handle
 * @data:      buffer to read data into
 * @len:       buffer len
 * @rdlen:     data in bytes of the returned record (output)
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_read_data_next(
	struct mpool       *mp,
	struct mpool_mlog  *mlh,
	void               *data,
	size_t              len,
	size_t             *rdlen);

/**
 * mpool_mlog_seek_read_data_next() - Reads the next record from mlog after
 * skipping bytes.
 * @mp:        mpool handle
 * @mlh:       mlog handle
 * skip        number of bytes to be skipped before read
 * @data:      buffer to read data into
 * @len:       buffer len
 * @rdlen:     data in bytes of the returned record (output)
 *
 * Return:
 *   %0 on success, <%0 on error
 *   If merr_errno() of the return value is EOVERFLOW, then the receive buffer
 *   "data" is too small and must be resized according to the value returned
 *   in "rdlen".
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_seek_read_data_next(
	struct mpool       *mp,
	struct mpool_mlog  *mlh,
	size_t              skip,
	void               *data,
	size_t              len,
	size_t             *rdlen);

/**
 * mpool_mlog_flush() - Flushes/syncs an mlog
 * @mp:        mpool handle
 * @mlh:       mlog handle
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_flush(
	struct mpool       *mp,
	struct mpool_mlog  *mlh);

/**
 * mpool_mlog_len() - Returns length of an mlog
 * @mp:        mpool handle
 * @mlh:       mlog handle
 * @len:       length of the mlog (output)
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_len(
	struct mpool       *mp,
	struct mpool_mlog  *mlh,
	size_t             *len);

/**
 * mpool_mlog_empty() - Returns if an mlog is empty
 * @mp:        mpool handle
 * @mlh:       mlog handle
 * @empty:     is the log empty? (output)
 *
 * Return:
 *   %0 on success, <%0 on error
 */
uint64_t
mpool_mlog_empty(
	struct mpool       *mp,
	struct mpool_mlog  *mlh,
	bool               *empty);

/**
 * mpool_mlog_erase() - Erase an mlog
 * @mp:        mpool handle
 * @mlh:       mlog handle
 * @mingen:    mininum generation number to use (pass 0 if not relevant)
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mlog_erase(
	struct mpool       *mp,
	struct mpool_mlog  *mlh,
	uint64_t            mingen);

/**
 * mpool_mlog_getprops() - Get properties of an mlog
 * @mp:        mpool handle
 * @mlh:       mlog handle
 * @props:     mlog properties (output)
 *
 * Return:
 *   %0 on success, <%0 on error
 */
uint64_t
mpool_mlog_getprops(
	struct mpool           *mp,
	struct mpool_mlog      *mlh,
	struct mlog_props      *props);

/*
 * MDC (Metadata Container) APIs
 */

/**
 * mdc_open_flags -
 * @MDC_OF_SKIP_SER: appends and reads are guaranteed to be serialized
 *                   outside of the MDC API
 */
enum mdc_open_flags {
	MDC_OF_SKIP_SER  = 0x1,
};

/**
 * mdc_capacity -
 * @mdt_captgt: capacity target for mlog in bytes
 * @mpt_spare:  true if alloc MDC from spare space
 */
struct mdc_capacity {
	uint64_t   mdt_captgt;
	bool       mdt_spare;
};

/**
 * mdc_props -
 * @mdc_objid1:
 * @mdc_objid2:
 * @mdc_alloc_cap:
 * @mdc_mclassp:
 */
struct mdc_props {
	uint64_t               mdc_objid1;
	uint64_t               mdc_objid2;
	uint64_t               mdc_alloc_cap;
	enum mp_media_classp   mdc_mclassp;
};

/**
 * mpool_mdc_alloc() - Alloc an MDC
 * @mp:         mpool handle
 * @logid1:     Mlog ID 1
 * @logid2:     Mlog ID 2
 * @mclassp:    media class
 * @ecparm:     erase coding parameters
 * @capreq:     capacity requirements
 */
/* MTF_MOCK */
uint64_t
mpool_mdc_alloc(
	struct mpool               *mp,
	uint64_t                   *logid1,
	uint64_t                   *logid2,
	enum mp_media_classp        mclassp,
	const struct mdc_capacity  *capreq,
	struct mdc_props           *props);

/**
 * mpool_mdc_commit() - Commit an MDC
 * @mp:       mpool handle
 * @logid1:   Mlog ID 1
 * @logid2:   Mlog ID 2
 */
/* MTF_MOCK */
uint64_t
mpool_mdc_commit(
	struct mpool           *mp,
	uint64_t                logid1,
	uint64_t                logid2);

/**
 * mpool_mdc_destroy() - Destroy a MDC
 * @mp:       mpool handle
 * @logid1:   Mlog ID 1
 * @logid2:   Mlog ID 2
 */
/* MTF_MOCK */
uint64_t
mpool_mdc_destroy(
	struct mpool           *mp,
	uint64_t                logid1,
	uint64_t                logid2);

/**
 * mpool_mdc_get_root() - Retrieve mpool root MDC OIDs
 * @mp:       mpool handle
 * @logid1:   Mlog ID 1
 * @logid2:   Mlog ID 2
 */
/* MTF_MOCK */
uint64_t
mpool_mdc_get_root(
	struct mpool           *mp,
	uint64_t               *logid1,
	uint64_t               *logid2);

/**
 * mpool_mdc_open() - Open MDC by OIDs
 * @mp:       mpool handle
 * @logid1:   Mlog ID 1
 * @logid2:   Mlog ID 2
 * @flags:    MDC Open flags (enum mdc_open_flags)
 * @mdc_out:  MDC handle
 */
/* MTF_MOCK */
uint64_t
mpool_mdc_open(
	struct mpool            *mp,
	uint64_t                 logid1,
	uint64_t                 logid2,
	uint8_t                  flags,
	struct mpool_mdc       **mdc_out);

/**
 * mpool_mdc_close() - Close MDC
 * @mdc:      MDC handle
 */
/* MTF_MOCK */
uint64_t
mpool_mdc_close(
	struct mpool_mdc   *mdc);

/**
 * mpool_mdc_sync() - Flush MDC content to media
 * @mdc:      MDC handle
 */
/* MTF_MOCK */
uint64_t
mpool_mdc_sync(
	struct mpool_mdc   *mdc);

/**
 * mpool_mdc_rewind() - Rewind MDC to first record
 * @mdc:      MDC handle
 */
/* MTF_MOCK */
uint64_t
mpool_mdc_rewind(
	struct mpool_mdc   *mdc);

/**
 * mpool_mdc_read() - Read next record from MDC
 * @mdc:      MDC handle
 * @data:     buffer to receive data
 * @len:      length of supplied buffer
 * @rdlen:    number of bytes read
 *
 * Return:
 *   If merr_errno() of the return value is EOVERFLOW, then the receive buffer
 *   "data" is too small and must be resized according to the value returned
 *   in "rdlen".
 */
/* MTF_MOCK */
uint64_t
mpool_mdc_read(
	struct mpool_mdc   *mdc,
	void               *data,
	size_t              len,
	size_t             *rdlen);

/**
 * mpool_mdc_append() - append record to MDC
 * @mdc:      MDC handle
 * @data:     data to write
 * @len:      length of data
 * @sync:     flag to defer return until IO is complete
 */
/* MTF_MOCK */
uint64_t
mpool_mdc_append(
	struct mpool_mdc   *mdc,
	void               *data,
	ssize_t             len,
	bool                sync);

/**
 * mpool_mdc_cstart() - Initiate MDC compaction
 * @mdc:      MDC handle
 *
 * Swap active (ostensibly full) and inactive (empty) mlogs
 * Append a compaction start marker to newly active mlog
 */
/* MTF_MOCK */
uint64_t
mpool_mdc_cstart(
	struct mpool_mdc   *mdc);

/**
 * mpool_mdc_cend() - End MDC compactions
 * @mdc:      MDC handle
 *
 * Append a compaction end marker to the active mlog
 */
/* MTF_MOCK */
uint64_t
mpool_mdc_cend(
	struct mpool_mdc   *mdc);

/**
 * mpool_mdc_usage() - Return estimate of active mlog usage
 * @mdc:      MDC handle
 * @usage:    Number of bytes used (includes overhead)
 */
/* MTF_MOCK */
uint64_t
mpool_mdc_usage(
	struct mpool_mdc   *mdc,
	size_t             *usage);

/* Mblock APIs
 */

/**
 * mpool_mblock_alloc() - allocate an mblock
 * @mp:		mpool
 * @mclassp:	media class
 * @spare:      allocate from spare zones
 * @mbh:	mblock handle (output)
 * @props:	properties of new mblock (output) - will be returned if the
 *		ptr is non-NULL
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mblock_alloc(
	struct mpool                   *mp,
	enum mp_media_classp		mclassp,
	bool                            spare,
	uint64_t                       *mbh,
	struct mblock_props            *props);

/* deprecated, do not use */
/* MTF_MOCK */
uint64_t
mpool_mblock_find_get(
	struct mpool           *mp,
	uint64_t                objid,
	uint64_t               *mbh,
	struct mblock_props    *props);

/**
 * mpool_mblock_find() -
 * @mp:     mpool
 * @objid:  Object ID for the mblock
 * @mbh:    mblock handle
 * @props:  mblock properties (returned if the ptr is non-NULL)
 *
 *   %0 on success, <%0 on error
 */
uint64_t
mpool_mblock_find(
	struct mpool           *mp,
	uint64_t                objid,
	uint64_t               *mbh,
	struct mblock_props    *props);

/* deprecated, do not use */
/* MTF_MOCK */
uint64_t
mpool_mblock_put(
	struct mpool   *mp,
	uint64_t        mbh);

/**
 * mpool_mblock_commit() - commit an mblock
 * @mp:  mpool
 * @mbh: mblock handle
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mblock_commit(
	struct mpool   *mp,
	uint64_t        mbh);

/**
 * mpool_mblock_abort() - abort an mblock
 * @mp:  mpool
 * @mbh: mblock handle
 *
 * mblock must have been allocated but not yet committed.
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mblock_abort(
	struct mpool   *mp,
	uint64_t        mbh);

/**
 * mpool_mblock_delete() - abort an mblock
 * @mp:  mpool
 * @mbh: mblock handle
 *
 * mblock must have been allocated but not yet committed.
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mblock_delete(
	struct mpool   *mp,
	uint64_t        mbh);

/**
 * mpool_mblock_getprops() - get properties of an mblock
 * @mp:     mpool
 * @mbh:    mblock handle
 * @props:  properties (output)
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mblock_getprops(
	struct mpool           *mp,
	uint64_t                mbh,
	struct mblock_props    *props);

/**
 * mpool_mblock_write_data() - write data to an mblock (wrapper)
 * @mp:              mpool
 * @sync_writes      flag to use sync writes only
 * @mbh:             mblock handle
 * @iov, @iov_cnt:   iovec containing data to be written
 * @pasyncio:        Async IO context
 *
 * Mblock writes are all or nothing.  Either all data is written to media, or
 * no data is written to media.  Hence, return code is success/fail instead of
 * the usual number of bytes written.
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mblock_write_data(
	struct mpool           *mp,
	bool                    sync_writes,
	uint64_t                mbh,
	struct iovec           *iov,
	int                     iov_cnt,
	struct mp_asyncctx_ioc *pasyncio);

/**
 * mpool_mblock_write() - write data to an mblock synchronously
 * @mp:              mpool
 * @mbh:             mblock handle
 * @iov, @iov_cnt:   iovec containing data to be written
 *
 * Mblock writes are all or nothing.  Either all data is written to media, or
 * no data is written to media.  Hence, return code is success/fail instead of
 * the usual number of bytes written.
 *
 * Return:
 *   %0 on success, <%0 on error
 */
uint64_t
mpool_mblock_write(
	struct mpool     *mp,
	uint64_t          mbh,
	struct iovec     *iov,
	int               iov_cnt);

/**
 * mpool_mblock_write_async() - write data to an mblock asynchronously
 * @mp:              mpool
 * @mbh:             mblock handle
 * @iov, @iov_cnt:   iovec containing data to be written
 * @pasyncio:        Async IO context
 *
 * Mblock writes are all or nothing.  Either all data is written to media, or
 * no data is written to media.  Hence, return code is success/fail instead of
 * the usual number of bytes written.
 * Async writes issues IO in 1MB chunks.  Pending buffers are
 * stored in a list. On first IO, the kernel allocates a context
 * object, the same context object is passed to the kernel for
 * subsequent IO. Periodically the flush routine is called
 * to make sure all pending IO are persisted and the list is
 * freed.
 * Return: %0 on success, <%0 on error
 */
uint64_t
mpool_mblock_write_async(
	struct mpool           *mp,
	uint64_t                mbh,
	struct iovec           *iov,
	int                     iov_cnt,
	struct mp_asyncctx_ioc *pasyncio);

/**
 * mpool_mblock_asyncio_flush() - Wait for pending IO persisted on IO
 * @mp:              mpool
 * @pasyncctx:       Context object holding IO.
 *
 * All the IO issued with the context are persisted on disk.
 *
 * Return:
 *   %0 on success, <%0 on error
 */
/* MTF_MOCK */
uint64_t
mpool_mblock_asyncio_flush(
	struct mpool           *mp,
	struct mp_asyncctx_ioc *pasyncctx);

/**
 * mpool_mblock_read() - read data from an mblock
 * @mp:             mpool
 * @mbh:            mblock handle
 * @iov, @iov_cnt:  iovec for output data
 * @offset:         PAGE aligned offset into the mblock
 *
 * Return:
 *   On failure: <%0  : -ERRNO
 *   On success: >=%0 : number of bytes read
 */
/* MTF_MOCK */
uint64_t
mpool_mblock_read(
	struct mpool     *mp,
	uint64_t          mbh,
	struct iovec     *iov,
	int               iov_cnt,
	size_t            offset);

/************* mcache stuff **********************************************/

/**
 * mpool_mcache_madvise() - Give advice about use of memory
 * @map:        mcache map handle
 * @mbidx:      logical mblock number in mcache map
 * @offset:     offset into the mblock specified by mbidx
 * @length:     see madvise(2)
 * @advice:     see madvise(2)
 *
 * Like madvise(2), but for mcache maps.
 *
 * Note that one can address the entire map (including holes) by
 * specifying zero for %mbidx, zero for %offset, and %SIZE_MAX for
 * %length.  In general, %SIZE_MAX may always be specified for %length,
 * in which case it addresses the map from the given mbidx based offset
 * to the end of the map.
 */
/* MTF_MOCK */
uint64_t
mpool_mcache_madvise(
	struct mpool_mcache_map    *map,
	uint                        mbidx,
	off_t                       offset,
	size_t                      length,
	int                         advice);

/**
 * mpool_mcache_purge() - Purge map
 *
 * @map: mcache map handle
 * @mp:  mp mpool
 */
/* MTF_MOCK */
uint64_t
mpool_mcache_purge(
	struct mpool_mcache_map    *map,
	const struct mpool         *mp);
/**
 * mpool_mcache_mincore() - Get VSS and RSS for the mcache map
 *
 * @map:        mcache map handle
 * @rssp:       ptr to count of resident pages in the map
 * @vssp:       ptr to count of virtual pages in the map
 *
 * Get the virtual and resident set sizes (in pages count)
 * for the given mcache map.
 */
/* MTF_MOCK */
uint64_t
mpool_mcache_mincore(
	struct mpool_mcache_map    *map,
	const struct mpool         *mp,
	size_t                     *rssp,
	size_t                     *vssp);

/**
 * mpool_mcache_getbase() - Get the base address of a memory-mapped
 *                       mblock in an mcache map
 * @map:        mcache map handle
 * @mbidx:      mcache map mblock index
 *
 * If the pages of an mcache map are contiguous in memory (as is the case in
 * user-space), return the the base address of the mapped mblock.  If the
 * pages are not contiguous, return NULL.
 */
/* MTF_MOCK */
void *
mpool_mcache_getbase(
	struct mpool_mcache_map    *map,
	const uint                  mbidx);


/**
 * mpool_mcache_getpages() - Get a vector of pages from a single mblock
 * @map:        mcache map handle
 * @pagec:      page count (len of @pagev array)
 * @mbidx:      mcache map mblock index
 * @offsetv:    vector of page offsets into objects/mblocks
 * @pagev:      vector of pointers to pages (output)
 *
 * mbidx is an index into the mbidv[] vector that was given
 * to mpool_mcache_create().
 *
 * Return:
 *   %0 on success, uint64_t on failure
 */
/* MTF_MOCK */
uint64_t
mpool_mcache_getpages(
	struct mpool_mcache_map    *map,
	const uint                  pagec,
	const uint                  mbidx,
	const size_t                offsetv[],
	void                       *pagev[]);

/**
 * mpool_mcache_getpagesv() - Get a vector of pages from a list of mblocks
 * @map:        mcache map handle
 * @mbidxv:     vector of mcache map mblock indices
 * @pagec:      page count (len of @pagev array)
 * @offsetv:    vector of page offsets into objects/mblocks
 * @pagev:      vector of pointers to pages (output)
 *
 * mbidxv is a vector of indices into the mbidv[] vector
 * that was given to mpool_mcache_create().
 *
 * Return:
 *   %0 on success, uint64_t on failure
 */
uint64_t
mpool_mcache_getpagesv(
	struct mpool_mcache_map    *map,
	const uint                  pagec,
	const uint                  mbidxv[],
	const size_t                offsetv[],
	void                       *pagev[]);

/**
 * mpool_mcache_mmap() - Create an mcache map
 * @mp:         handle for the mpool
 * @mbidc:      mblock ID count
 * @mbidv:      vector of mblock IDs
 * @advice:
 * @mapp:       pointer to (opaque) mpool_mcache_map ptr
 *
 * Create an mcache map for the list of given mblock IDs
 * and returns a handle to it via *mapp.
 */
/* MTF_MOCK */
uint64_t
mpool_mcache_mmap(
	struct mpool               *mp,
	size_t                      mbidc,
	uint64_t                   *mbidv,
	enum mpc_vma_advice         advice,
	struct mpool_mcache_map    **mapp);

/**
 * mpool_mcache_munmap() - munmap an mcache mmap
 */
/* MTF_MOCK */
uint64_t
mpool_mcache_munmap(
	struct mpool_mcache_map   *map);

#if defined(HSE_UNIT_TEST_MODE) && HSE_UNIT_TEST_MODE == 1
#include "mpool_ut.h"
#endif /* HSE_UNIT_TEST_MODE */

#endif /* MPOOL_H */
