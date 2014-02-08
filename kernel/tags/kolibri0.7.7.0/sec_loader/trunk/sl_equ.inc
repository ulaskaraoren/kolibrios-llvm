; Copyright (c) 2008-2009, <Lrz>
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions are met:
;       * Redistributions of source code must retain the above copyright
;       notice, this list of conditions and the following disclaimer.
;       * Redistributions in binary form must reproduce the above copyright
;       notice, this list of conditions and the following disclaimer in the
;       documentation and/or other materials provided with the distribution.
;       * Neither the name of the <organization> nor the
;       names of its contributors may be used to endorse or promote products
;       derived from this software without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY Alexey Teplov aka <Lrz> ''AS IS'' AND ANY
; EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
; DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
; (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
; ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
; SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;*****************************************************************************
; ���������������
DEBUG equ 1         			;���������� � ���������� ����������� =1 ��� ���������� ��������� =0
loop_read_startos_file  equ     3	;���-�� ������� ������� ����� callback ������ ���� ������������ ����2
root_dir_entry_count	equ	224	;���-�� ��������� � �������� �����������
;point_to_fat_struc	equ	0xA000	;��������� �����, ���� ����� ��������� Fat �������, � ����� ���������� �� 1 ��
ini_data_       	equ     0x2000  ;���� ��� �������� ���� �������� ��������, ��� ���������� �������������� ������
size_show_section 	equ 	18
default_timeout_value	equ	5	;default value to timeout is will was some errors
flag_found_default	equ	0x1	;default value is found
flag_found_timeout	equ	0x2	;timeout value is found
flag_found_LM		equ	0x1	;found LM value
flag_found_RS		equ	0x2	;found RS value
flag_found_GTRFMS	equ	0x4	;found type RamFS
flag_found_RamdiskSector equ	0x8	;found RamdiskSector
flag_found_RamdiskCluster equ   0x16	;found RamdiskCluster
;statick data ��� ������ �� ���������������� � ������� ���������� ���� ���������.
save_cx		equ	word [bp-2]	;save cx size ini file
ret_on_ch	equ	word [bp-4]     ;point to return   ����������� ��������
save_cx_d	equ	word [bp-6]     ;save cx - size default section and working section                                
status_flag	equ	word [bp-8]	;status flag
point_loader    equ	word [bp-10]
point_default   equ	word [bp-12]    ;point to default

;������ ������� �������� �� ����� ���������� � ������� ����� ���� �������������� � �������� ���������� ���������.
point_to_hframe equ	word [bp-14]    ;point on start frame (for change section)
point_to_1	equ	word [bp-16]
point_to_2	equ	word [bp-18]
point_to_3	equ	word [bp-20]
point_to_4	equ	word [bp-22]
point_to_5	equ	word [bp-24]
point_to_6	equ	word [bp-26]
point_to_7	equ	word [bp-28]
point_to_8	equ	word [bp-30]
point_to_9	equ	word [bp-32]
point_to_10	equ	word [bp-34]
point_to_11	equ	word [bp-36]
point_to_12	equ	word [bp-38]
point_to_13	equ	word [bp-40]
point_to_14	equ	word [bp-42]
point_to_15	equ	word [bp-44]
point_to_16	equ	word [bp-46]
point_to_16	equ	word [bp-48]
point_to_17	equ	word [bp-50]
point_to_18	equ	word [bp-52]
;here array for fast scroling  16 word - poin to start section
point_to_point_def	equ	word [bp-54]
point_to_eframe equ	word [bp-56]    ;point on point frame



; ��� ����������� ��������� ��������� ��� cx � di ��� �������� �� ��������� ����� ��� ������ ������
find_sec_di	equ	word [bp-58]	;��� ����� ��������� di
info_real_mode_size	equ 	word [bp-60];��� ��������� ���������� � ������� ������� �.�. ������, ����� ������ ������� �������� ����� ��������
free_ad_memory  equ	word	[bp-62] ;������� � ��� ����������� ������ ��� ������������ ��� ����� � �������� �������
show_errors_sect equ    word    [bp-64] ;��������� ������� ������ ���� ������ ��� ������ ���������� ������. 
save_descript_size	equ  	word	[bp-66]	;save descript size previos section �������� ������ ���������� ������ ������� ��������
save_ramdisksize	equ 	dword	[bp-70] ;save size of ramdisk in byte
save_file_size		equ	dword   [bp-74] ;save size of reading file
set_ramfs		equ	word	[bp-76]	;������������ ��� �������� �������,����� ��� ������������ ��� �����
point_next_fat_str	equ	word	[bp-78] ;��������� �� ��������� ������� fat �������
size_root_dir		equ	word	[bp-80] ;���-�� ��������� � �������� �� 512 ���� �������� ����������
firstDataSect		equ	word	[bp-82] ;������ ������ ������ � ������� �� 0
DataClasters		equ	word	[bp-84] ;������ ������� ��������� ��� ������ ������ � ���������.
point_to_free_root	equ	word	[bp-86]	;��������� �� ��������� ������ ������ � ��� ���
point_to_dest_file_name equ	word	[bp-88]	;��������� �� ������ ����� ����� ����������. � ������� es:point_to_dest_file_name, ��� es =0x2000
data_offset		equ	word	[bp-90] ;�������� � ��������� ��� ���������� ������ �.� ����������� �� 1-� ��
first_input		equ	word	[bp-92]	;���� ��� ������ � �������������� �����.
save_di_RAMDISK		equ	word	[bp-94]	;�������� di -��������� ��� ��������� ������
save_cx_RAMDISK		equ	word	[bp-96]	;�������� ������ ������� ������
status_flag_loader_f	equ	word	[bp-98] ;�������� ��������� ����������� �������� �����
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;������ ������� ������������ ��� ��������� ������, �.�. ����� ������� Enter, ��� �� �������� ��������� � �������������� �����
;��� ��������, ���������� ������������� ��������� ��� �.�. ���������� � 0�1000:0000