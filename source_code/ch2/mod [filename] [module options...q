INSMOD(8)                                         insmod                                        INSMOD(8)

NNAAMMEE
       insmod - Simple program to insert a module into the Linux Kernel

SSYYNNOOPPSSIISS
       iinnssmmoodd [_f_i_l_e_n_a_m_e] [_m_o_d_u_l_e _o_p_t_i_o_n_s...]

DDEESSCCRRIIPPTTIIOONN
       iinnssmmoodd is a trivial program to insert a module into the kernel. Most users will want to use
       mmooddpprroobbee(8) instead, which is more clever and can handle module dependencies.

       Only the most general of error messages are reported: as the work of trying to link the module is
       now done inside the kernel, the ddmmeessgg usually gives more information about errors.

CCOOPPYYRRIIGGHHTT
       This manual page originally Copyright 2002, Rusty Russell, IBM Corporation. Maintained by Jon
       Masters and others.

SSEEEE AALLSSOO
       mmooddpprroobbee(8), rrmmmmoodd(8), llssmmoodd(8), mmooddiinnffoo(8) ddeeppmmoodd(8)

AAUUTTHHOORRSS
       JJoonn MMaasstteerrss <jcm@jonmasters.org>
           Developer

       LLuuccaass DDee MMaarrcchhii <lucas.de.marchi@gmail.com>
           Developer

kmod                                            03/12/2020                                      INSMOD(8)
