/*
 * Copyright (c) 2004-2010 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2011 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2006-2013 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2009-2012 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2011      Oak Ridge National Labs.  All rights reserved.
 * Copyright (c) 2013-2015 Intel, Inc.  All rights reserved.
 * Copyright (c) 2015      Mellanox Technologies, Inc.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#include <private/autogen/config.h>
#include <pmix.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "src/class/pmix_object.h"
#include "src/buffer_ops/types.h"
#include "src/util/argv.h"
#include "src/util/output.h"
#include "src/util/printf.h"

static pmix_proc_t myproc;
static bool completed;

static void notification_fn(pmix_status_t status,
                            pmix_proc_t procs[], size_t nprocs,
                            pmix_info_t info[], size_t ninfo)
{
    pmix_output(0, "Client %s:%d NOTIFIED with status %d", myproc.nspace, myproc.rank, status);
    completed = true;
}

int main(int argc, char **argv)
{
    int rc;
    pmix_value_t value;
    pmix_value_t *val = &value;
    pmix_proc_t proc;
    uint32_t nprocs;
    
    /* init us */
    if (PMIX_SUCCESS != (rc = PMIx_Init(&myproc))) {
        pmix_output(0, "Client ns %s rank %d: PMIx_Init failed: %d", myproc.nspace, myproc.rank, rc);
        exit(0);
    }
    pmix_output(0, "Client ns %s rank %d: Running", myproc.nspace, myproc.rank);

    /* get our universe size */
    if (PMIX_SUCCESS != (rc = PMIx_Get(&myproc, PMIX_UNIV_SIZE, NULL, 0, &val))) {
        pmix_output(0, "Client ns %s rank %d: PMIx_Get universe size failed: %d", myproc.nspace, myproc.rank, rc);
        goto done;
    }
    nprocs = val->data.uint32;
    PMIX_VALUE_RELEASE(val);
    pmix_output(0, "Client %s:%d universe size %d", myproc.nspace, myproc.rank, nprocs);
    completed = false;
    
    /* register our errhandler */
    PMIx_Register_errhandler(NULL, 0, notification_fn);
    
    /* call fence to sync */
    PMIX_PROC_CONSTRUCT(&proc);
    (void)strncpy(proc.nspace, myproc.nspace, PMIX_MAX_NSLEN);
    proc.rank = PMIX_RANK_WILDCARD;
    if (PMIX_SUCCESS != (rc = PMIx_Fence(&proc, 1, NULL, 0))) {
        pmix_output(0, "Client ns %s rank %d: PMIx_Fence failed: %d", myproc.nspace, myproc.rank, rc);
        goto done;
    }
    
    /* rank=0 calls abort */
    if (0 == myproc.rank) {
        PMIx_Abort(PMIX_ERR_OUT_OF_RESOURCE, "Eat rocks",
                   &proc, 1);
        pmix_output(0, "Client ns %s rank %d: Abort called", myproc.nspace, myproc.rank);
    }
    /* everyone simply waits */
    while (!completed) {
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 100000;
        nanosleep(&ts, NULL);
    }

 done:
    /* finalize us */
    pmix_output(0, "Client ns %s rank %d: Finalizing", myproc.nspace, myproc.rank);
    PMIx_Deregister_errhandler();
    
    if (PMIX_SUCCESS != (rc = PMIx_Finalize())) {
        fprintf(stderr, "Client ns %s rank %d:PMIx_Finalize failed: %d\n", myproc.nspace, myproc.rank, rc);
    } else {
        fprintf(stderr, "Client ns %s rank %d:PMIx_Finalize successfully completed\n", myproc.nspace, myproc.rank);
    }
    fflush(stderr);
    return(0);
}
