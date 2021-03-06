/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2011-2012 Cisco Systems, Inc.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "ompi_config.h"

#include "ompi/mpi/fortran/mpif-h/bindings.h"

#if OPAL_HAVE_WEAK_SYMBOLS && OMPI_PROFILE_LAYER
#pragma weak PMPI_WIN_COMPLETE = ompi_win_complete_f
#pragma weak pmpi_win_complete = ompi_win_complete_f
#pragma weak pmpi_win_complete_ = ompi_win_complete_f
#pragma weak pmpi_win_complete__ = ompi_win_complete_f

#pragma weak PMPI_Win_complete_f = ompi_win_complete_f
#pragma weak PMPI_Win_complete_f08 = ompi_win_complete_f
#elif OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (PMPI_WIN_COMPLETE,
                           pmpi_win_complete,
                           pmpi_win_complete_,
                           pmpi_win_complete__,
                           pompi_win_complete_f,
                           (MPI_Fint *win, MPI_Fint *ierr),
                           (win, ierr) )
#endif

#if OPAL_HAVE_WEAK_SYMBOLS
#pragma weak MPI_WIN_COMPLETE = ompi_win_complete_f
#pragma weak mpi_win_complete = ompi_win_complete_f
#pragma weak mpi_win_complete_ = ompi_win_complete_f
#pragma weak mpi_win_complete__ = ompi_win_complete_f

#pragma weak MPI_Win_complete_f = ompi_win_complete_f
#pragma weak MPI_Win_complete_f08 = ompi_win_complete_f
#endif

#if ! OPAL_HAVE_WEAK_SYMBOLS && ! OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (MPI_WIN_COMPLETE,
                           mpi_win_complete,
                           mpi_win_complete_,
                           mpi_win_complete__,
                           ompi_win_complete_f,
                           (MPI_Fint *win, MPI_Fint *ierr),
                           (win, ierr) )
#endif


#if OMPI_PROFILE_LAYER && ! OPAL_HAVE_WEAK_SYMBOLS
#include "ompi/mpi/fortran/mpif-h/profile/defines.h"
#endif

void ompi_win_complete_f(MPI_Fint *win, MPI_Fint *ierr)
{
    int c_ierr;
    MPI_Win c_win;

    c_win = MPI_Win_f2c(*win);

    c_ierr = MPI_Win_complete(c_win);
    if (NULL != ierr) *ierr = OMPI_INT_2_FINT(c_ierr);
}
