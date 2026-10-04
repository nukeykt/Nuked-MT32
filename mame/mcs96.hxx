void mcs96_device::skip_immed_1b_full()
{
	OP1 = read_pc();
	next(4);
}

void mcs96_device::clr_direct_1w_full()
{
	OP1 = read_pc();
	reg_w16(OP1, 0x0000);
	next(4);
}

void mcs96_device::not_direct_1w_full()
{
	OP1 = read_pc();
	TMP = ~reg_r16(OP1);
	set_nz16(TMP);
	reg_w16(OP1, TMP);
	next(4);
}

void mcs96_device::neg_direct_1w_full()
{
	OP1 = read_pc();
	TMP = reg_r16(OP1);
	reg_w16(OP1, do_sub(0, TMP));
	next(4);
}

void mcs96_device::xch_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
}

void mcs96_device::dec_direct_1w_full()
{
	OP1 = read_pc();
	TMP = reg_r16(OP1);
	reg_w16(OP1, do_sub(TMP, 1));
	next(4);
}

void mcs96_device::ext_direct_1w_full()
{
	OP1 = read_pc();
	OP1 &= 0xfc;
	TMP = int16_t(reg_r16(OP1));
	set_nz16(TMP);
	reg_w16(OP1+2, TMP >> 16);
	next(4);
}

void mcs96_device::inc_direct_1w_full()
{
	OP1 = read_pc();
	TMP = reg_r16(OP1);
	reg_w16(OP1, do_add(TMP, 1));
	next(4);
}

void mcs96_device::shr_immed_or_reg_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	if(OP1 >= 0x10) {
		OP1 = reg_r8(OP1) & 0x1f;
	}
	TMP = reg_r16(OP2);
	PSW &= ~(F_Z|F_N|F_C|F_V|F_ST);
	if(OP1 >= 2 && (TMP & (0xffff >> (OP1 <= 16 ? 17-OP1 : 0))))
		PSW |= F_ST;
	if(OP1 >= 1 && OP1 <= 16 && (TMP & (0x0001 << (OP1-1))))
		PSW |= F_C;
	TMP = uint16_t(TMP) >> OP1;
	if(!TMP)
		PSW |= F_Z;
	else if(int16_t(TMP) < 0)
		PSW |= F_N;
	reg_w16(OP2, TMP);
	next(OP1 ? 7+OP1 : 8);
}

void mcs96_device::shl_immed_or_reg_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	if(OP1 >= 0x10) {
		OP1 = reg_r8(OP1) & 0x1f;
	}
	TMP = reg_r16(OP2);
	PSW &= ~(F_Z|F_N|F_C|F_V|F_ST);
	if(OP1 >= 2 && (TMP & (0xffff << (OP1 <= 16 ? 17-OP1 : 0))))
		PSW |= F_ST;
	if(OP1 >= 1 && OP1 <= 16 && (TMP & (0x8000 >> (OP1-1))))
		PSW |= F_C;
	TMP = uint16_t(TMP << OP1);
	if(!TMP)
		PSW |= F_Z;
	else if(int16_t(TMP) < 0)
		PSW |= F_N;
	reg_w16(OP2, TMP);
	next(OP1 ? 7+OP1 : 8);
}

void mcs96_device::shra_immed_or_reg_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	if(OP1 >= 0x10) {
		OP1 = reg_r8(OP1) & 0x1f;
	}
	TMP = reg_r16(OP2);
	PSW &= ~(F_Z|F_N|F_C|F_V|F_ST);
	if(OP1 >= 2 && (TMP & (0xffff >> (OP1 <= 16 ? 17-OP1 : 0))))
		PSW |= F_ST;
	if(OP1 >= 1 && OP1 <= 16 && (TMP & (0x0001 << (OP1-1))))
		PSW |= F_C;
	TMP = int16_t(TMP) >> OP1;
	if(!TMP)
		PSW |= F_Z;
	else if(int16_t(TMP) < 0)
		PSW |= F_N;
	reg_w16(OP2, TMP);
	next(OP1 ? 7+OP1 : 8);
}

void mcs96_device::shrl_immed_or_reg_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	if(OP1 >= 0x10) {
		OP1 = reg_r8(OP1) & 0x1f;
	}
	OP2 &= 0xfc;
	TMP = reg_r16(OP2);
	TMP |= reg_r16(OP2+2) << 16;
	PSW &= ~(F_Z|F_N|F_C|F_V|F_ST);
	if(OP1 >= 2 && (TMP & (0xffffffff >> (33-OP1))))
		PSW |= F_ST;
	if(OP1 >= 1 && (TMP & (0x00000001 << (OP1-1))))
		PSW |= F_C;
	TMP = TMP >> OP1;
	if(!TMP)
		PSW |= F_Z;
	else if(int32_t(TMP) < 0)
		PSW |= F_N;
	reg_w16(OP2, TMP);
	reg_w16(OP2+2, TMP >> 16);
	next(OP1 ? 7+OP1 : 8);
}

void mcs96_device::shll_immed_or_reg_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	if(OP1 >= 0x10) {
		OP1 = reg_r8(OP1) & 0x1f;
	}
	OP2 &= 0xfc;
	TMP = reg_r16(OP2);
	TMP |= reg_r16(OP2+2) << 16;
	PSW &= ~(F_Z|F_N|F_C|F_V|F_ST);
	if(OP1 >= 2 && (TMP & (0xffffffff << (33-OP1))))
		PSW |= F_ST;
	if(OP1 >= 1 && (TMP & (0x80000000 >> (OP1-1))))
		PSW |= F_C;
	TMP = TMP << OP1;
	if(!TMP)
		PSW |= F_Z;
	else if(int32_t(TMP) < 0)
		PSW |= F_N;
	reg_w16(OP2, TMP);
	reg_w16(OP2+2, TMP >> 16);
	next(OP1 ? 7+OP1 : 8);
}

void mcs96_device::shral_immed_or_reg_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	if(OP1 >= 0x10) {
		OP1 = reg_r8(OP1) & 0x1f;
	}
	OP2 &= 0xfc;
	TMP = reg_r16(OP2);
	TMP |= reg_r16(OP2+2) << 16;
	PSW &= ~(F_Z|F_N|F_C|F_V|F_ST);
	if(OP1 >= 2 && (TMP & (0xffffffff >> (33-OP1))))
		PSW |= F_ST;
	if(OP1 >= 1 && (TMP & (0x00000001 << (OP1-1))))
		PSW |= F_C;
	TMP = int32_t(TMP) >> OP1;
	if(!TMP)
		PSW |= F_Z;
	else if(int32_t(TMP) < 0)
		PSW |= F_N;
	reg_w16(OP2, TMP);
	reg_w16(OP2+2, TMP >> 16);
	next(OP1 ? 7+OP1 : 8);
}

void mcs96_device::norml_direct_2e_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP2 &= 0xfc;
	TMP = reg_r16(OP2);
	TMP |= reg_r16(OP2+2) << 16;
	for(OP3 = 0; OP3 < 31 && int32_t(TMP) >= 0; TMP <<= 1, OP3++);
	PSW &= ~(F_Z|F_N|F_C);
	if(!TMP)
		PSW |= F_Z;
	reg_w8(OP1, OP3);
	reg_w16(OP2, TMP);
	reg_w16(OP2+2, TMP >> 16);
	next(11+OP3);
}

void mcs96_device::clrb_direct_1b_full()
{
	OP1 = read_pc();
	reg_w8(OP1, 0x00);
	next(4);
}

void mcs96_device::notb_direct_1b_full()
{
	OP1 = read_pc();
	TMP = ~reg_r8(OP1);
	set_nz8(TMP);
	reg_w8(OP1, TMP);
	next(4);
}

void mcs96_device::negb_direct_1b_full()
{
	OP1 = read_pc();
	TMP = reg_r8(OP1);
	reg_w8(OP1, do_subb(0, TMP));
	next(4);
}

void mcs96_device::xchb_direct_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
}

void mcs96_device::decb_direct_1b_full()
{
	OP1 = read_pc();
	TMP = reg_r8(OP1);
	reg_w8(OP1, do_subb(TMP, 1));
	next(4);
}

void mcs96_device::extb_direct_1b_full()
{
	OP1 = read_pc();
	OP1 &= 0xfe;
	TMP = int8_t(reg_r8(OP1));
	set_nz8(TMP);
	reg_w16(OP1, TMP);
	next(4);
}

void mcs96_device::incb_direct_1b_full()
{
	OP1 = read_pc();
	TMP = reg_r8(OP1);
	reg_w8(OP1, do_addb(TMP, 1));
	next(4);
}

void mcs96_device::shrb_immed_or_reg_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	if(OP1 >= 0x10) {
		OP1 = reg_r8(OP1) & 0x1f;
	}
	TMP = reg_r8(OP2);
	PSW &= ~(F_Z|F_N|F_C|F_V|F_ST);
	if(OP1 >= 2 && (TMP & (0xff >> (OP1 <= 8 ? 9-OP1 : 0))))
		PSW |= F_ST;
	if(OP1 >= 1 && OP1 <= 8 && (TMP & (0x01 << (OP1-1))))
		PSW |= F_C;
	TMP = uint8_t(TMP) >> OP1;
	if(!TMP)
		PSW |= F_Z;
	else if(int8_t(TMP) < 0)
		PSW |= F_N;
	reg_w8(OP2, TMP);
	next(OP1 ? 7+OP1 : 8);
}

void mcs96_device::shlb_immed_or_reg_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	if(OP1 >= 0x10) {
		OP1 = reg_r8(OP1) & 0x1f;
	}
	TMP = reg_r8(OP2);
	PSW &= ~(F_Z|F_N|F_C|F_V|F_ST);
	if(OP1 >= 2 && (TMP & (0xff << (OP1 <= 8 ? 9-OP1 : 0))))
		PSW |= F_ST;
	if(OP1 >= 1 && OP1 <= 8 && (TMP & (0x80 >> (OP1-1))))
		PSW |= F_C;
	TMP = uint8_t(TMP << OP1);
	if(!TMP)
		PSW |= F_Z;
	else if(int8_t(TMP) < 0)
		PSW |= F_N;
	reg_w8(OP2, TMP);
	next(OP1 ? 7+OP1 : 8);
}

void mcs96_device::shrab_immed_or_reg_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	if(OP1 >= 0x10) {
		OP1 = reg_r8(OP1) & 0x1f;
	}
	TMP = reg_r8(OP2);
	PSW &= ~(F_Z|F_N|F_C|F_V|F_ST);
	if(OP1 >= 2 && (TMP & (0xff >> (OP1 <= 8 ? 9-OP1 : 0))))
		PSW |= F_ST;
	if(OP1 >= 1 && OP1 <= 8 && (TMP & (0x01 << (OP1-1))))
		PSW |= F_C;
	TMP = uint8_t(int8_t(TMP) >> OP1);
	if(!TMP)
		PSW |= F_Z;
	else if(int8_t(TMP) < 0)
		PSW |= F_N;
	reg_w8(OP2, TMP);
	next(OP1 ? 7+OP1 : 8);
}

void mcs96_device::sjmp_rel11_full()
{
	OP1 = read_pc();
	OP1 = OP1 | ((inst_state & 7) << 8);
	if(OP1 & 0x400)
		OP1 |= 0xfc00;
	PC += OP1;
	next(8);
}

void mcs96_device::scall_rel11_full()
{
	OP1 = read_pc();
	OP1 = OP1 | ((inst_state & 7) << 8);
	if(OP1 & 0x400)
		OP1 |= 0xfc00;
	TMP = reg_r16(0x18);
	TMP -= 2;
	reg_w16(0x18, TMP);
	any_w16(TMP, PC);
	PC += OP1;
	next(13); // real is 13/16 depending on sp's position
}

void mcs96_device::jbc_brrel8_full()
{
	OP2 = read_pc();
	OP1 = int8_t(read_pc());
	TMP = reg_r8(OP2);
	if(!((TMP >> (inst_state & 7)) & 1)) {
		PC += OP1;
		next(9);
	} else {
		next(5);
	}
}

void mcs96_device::jbs_brrel8_full()
{
	OP2 = read_pc();
	OP1 = int8_t(read_pc());
	TMP = reg_r8(OP2);
	if((TMP >> (inst_state & 7)) & 1) {
		PC += OP1;
		next(9);
	} else {
		next(5);
	}
}

void mcs96_device::and_direct_3w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = reg_r16(OP1);
	TMP &= reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP3, TMP);
	next(5);
}

void mcs96_device::and_immed_3w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = OP1 & reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP3, TMP);
	next(6);
}

void mcs96_device::and_indirect_3w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r16(OP1);
	TMP &= reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP3, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(8); // +4 when external
	} else {
		next(7); // +4 when external
	}
}

void mcs96_device::and_indexed_3w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r16(OP1);
	TMP &= reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP3, TMP);
	next(OPI & 0x01 ? 8 : 7);
}

void mcs96_device::add_direct_3w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = reg_r16(OP1);
	TMP = do_add(reg_r16(OP2), TMP);
	reg_w16(OP3, TMP);
	next(5);
}

void mcs96_device::add_immed_3w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = do_add(reg_r16(OP2), OP1);
	reg_w16(OP3, TMP);
	next(6);
}

void mcs96_device::add_indirect_3w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r16(OP1);
	TMP = do_add(reg_r16(OP2), TMP);
	reg_w16(OP3, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(8); // +4 when external
	} else {
		next(7); // +4 when external
	}
}

void mcs96_device::add_indexed_3w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r16(OP1);
	TMP = do_add(reg_r16(OP2), TMP);
	reg_w16(OP3, TMP);
	next(OPI & 0x01 ? 8 : 7);
}

void mcs96_device::sub_direct_3w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = reg_r16(OP1);
	TMP = do_sub(reg_r16(OP2), TMP);
	reg_w16(OP3, TMP);
	next(5);
}

void mcs96_device::sub_immed_3w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = do_sub(reg_r16(OP2), OP1);
	reg_w16(OP3, TMP);
	next(6);
}

void mcs96_device::sub_indirect_3w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r16(OP1);
	TMP = do_sub(reg_r16(OP2), TMP);
	reg_w16(OP3, TMP);
	next(OPI & 0x01 ? 8 : 7);
}

void mcs96_device::sub_indexed_3w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r16(OP1);
	TMP = do_sub(reg_r16(OP2), TMP);
	reg_w16(OP3, TMP);
	next(OPI & 0x01 ? 8 : 7);
}

void mcs96_device::mulu_direct_3w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = reg_r16(OP1);
	TMP *= reg_r16(OP2);
	OP3 &= 0xfc;
	reg_w16(OP3, TMP);
	reg_w16(OP3+2, TMP >> 16);
	next(26);
}

void mcs96_device::mulu_immed_3w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = OP1 * reg_r16(OP2);
	OP3 &= 0xfc;
	reg_w16(OP3, TMP);
	reg_w16(OP3+2, TMP >> 16);
	next(27);
}

void mcs96_device::mulu_indirect_3w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r16(OP1);
	TMP *= reg_r16(OP2);
	OP3 &= 0xfc;
	reg_w16(OP3, TMP);
	reg_w16(OP3+2, TMP >> 16);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(29); // +4 when external
	} else {
		next(28); // +4 when external
	}
}

void mcs96_device::mulu_indexed_3w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r16(OP1);
	TMP *= reg_r16(OP2);
	OP3 &= 0xfc;
	reg_w16(OP3, TMP);
	reg_w16(OP3+2, TMP >> 16);
	next(OPI & 0x01 ? 29 : 28);
}

void mcs96_device::mul_direct_3w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = reg_r16(OP1);
	TMP = int16_t(reg_r16(OP2)) * int16_t(TMP);
	OP3 &= 0xfc;
	reg_w16(OP3, TMP);
	reg_w16(OP3+2, TMP >> 16);
	next(30);
}

void mcs96_device::mul_immed_3w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = int16_t(OP1) * int16_t(reg_r16(OP2));
	OP3 &= 0xfc;
	reg_w16(OP3, TMP);
	reg_w16(OP3+2, TMP >> 16);
	next(31);
}

void mcs96_device::mul_indirect_3w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r16(OP1);
	TMP = int16_t(reg_r16(OP2)) * int16_t(TMP);
	OP3 &= 0xfc;
	reg_w16(OP3, TMP);
	reg_w16(OP3+2, TMP >> 16);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(33); // +4 when external
	} else {
		next(32); // +4 when external
	}
}

void mcs96_device::mul_indexed_3w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r16(OP1);
	TMP = int16_t(reg_r16(OP2)) * int16_t(TMP);
	OP3 &= 0xfc;
	reg_w16(OP3, TMP);
	reg_w16(OP3+2, TMP >> 16);
	next(OPI & 0x01 ? 33 : 32);
}

void mcs96_device::andb_direct_3b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = reg_r8(OP1);
	TMP &= reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP3, TMP);
	next(5);
}

void mcs96_device::andb_immed_3b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = OP1 & reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP3, TMP);
	next(5);
}

void mcs96_device::andb_indirect_3b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r8(OP1);
	TMP &= reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP3, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(8); // +4 when external
	} else {
		next(7); // +4 when external
	}
}

void mcs96_device::andb_indexed_3b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r8(OP1);
	TMP &= reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP3, TMP);
	next(OPI & 0x01 ? 8 : 7);
}

void mcs96_device::addb_direct_3b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = reg_r8(OP1);
	TMP = do_addb(reg_r8(OP2), TMP);
	reg_w8(OP3, TMP);
	next(5);
}

void mcs96_device::addb_immed_3b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = do_addb(reg_r8(OP2), OP1);
	reg_w8(OP3, TMP);
	next(5);
}

void mcs96_device::addb_indirect_3b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r8(OP1);
	TMP = do_addb(reg_r8(OP2), TMP);
	reg_w8(OP3, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(8); // +4 when external
	} else {
		next(7); // +4 when external
	}
}

void mcs96_device::addb_indexed_3b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r8(OP1);
	TMP = do_addb(reg_r8(OP2), TMP);
	reg_w8(OP3, TMP);
	next(OPI & 0x01 ? 8 : 7);
}

void mcs96_device::subb_direct_3b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = reg_r8(OP1);
	TMP = do_subb(reg_r8(OP2), TMP);
	reg_w8(OP3, TMP);
	next(5);
}

void mcs96_device::subb_immed_3b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = do_subb(reg_r8(OP2), OP1);
	reg_w8(OP3, TMP);
	next(5);
}

void mcs96_device::subb_indirect_3b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r8(OP1);
	TMP = do_subb(reg_r8(OP2), TMP);
	reg_w8(OP3, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(8); // +4 when external
	} else {
		next(7); // +4 when external
	}
}

void mcs96_device::subb_indexed_3b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r8(OP1);
	TMP = do_subb(reg_r8(OP2), TMP);
	reg_w8(OP3, TMP);
	next(OPI & 0x01 ? 8 : 7);
}

void mcs96_device::mulub_direct_3e_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = reg_r8(OP1);
	TMP *= reg_r8(OP2);
	reg_w16(OP3, TMP);
	next(18);
}

void mcs96_device::mulub_immed_3e_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = OP1 * reg_r8(OP2);
	reg_w16(OP3, TMP);
	next(18);
}

void mcs96_device::mulub_indirect_3e_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r8(OP1);
	TMP *= reg_r8(OP2);
	reg_w16(OP3, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(21); // +4 when external
	} else {
		next(20); // +4 when external
	}
}

void mcs96_device::mulub_indexed_3e_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r8(OP1);
	TMP *= reg_r8(OP2);
	reg_w16(OP3, TMP);
	next(OPI & 0x01 ? 21 : 20);
}

void mcs96_device::mulb_direct_3e_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = reg_r8(OP1);
	TMP = int8_t(reg_r8(OP2)) * int8_t(TMP);
	reg_w16(OP3, TMP);
	next(22);
}

void mcs96_device::mulb_immed_3e_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = int8_t(OP1) * int8_t(reg_r8(OP2));
	reg_w16(OP3, TMP);
	next(22);
}

void mcs96_device::mulb_indirect_3e_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r8(OP1);
	TMP = int8_t(reg_r8(OP2)) * int8_t(TMP);
	reg_w16(OP3, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(25); // +4 when external
	} else {
		next(24); // +4 when external
	}
}

void mcs96_device::mulb_indexed_3e_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP3 = read_pc();
	TMP = any_r8(OP1);
	TMP = int8_t(reg_r8(OP2)) * int8_t(TMP);
	reg_w16(OP3, TMP);
	next(OPI & 0x01 ? 25 : 24);
}

void mcs96_device::and_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r16(OP1);
	TMP &= reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP2, TMP);
	next(4);
}

void mcs96_device::and_immed_2w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	TMP = OP1 & reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP2, TMP);
	next(5);
}

void mcs96_device::and_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP &= reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::and_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP &= reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::add_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r16(OP1);
	TMP = do_add(reg_r16(OP2), TMP);
	reg_w16(OP2, TMP);
	next(4);
}

void mcs96_device::add_immed_2w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	TMP = do_add(reg_r16(OP2), OP1);
	reg_w16(OP2, TMP);
	next(5);
}

void mcs96_device::add_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP = do_add(reg_r16(OP2), TMP);
	reg_w16(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::add_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP = do_add(reg_r16(OP2), TMP);
	reg_w16(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::sub_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r16(OP1);
	TMP = do_sub(reg_r16(OP2), TMP);
	reg_w16(OP2, TMP);
	next(4);
}

void mcs96_device::sub_immed_2w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	TMP = do_sub(reg_r16(OP2), OP1);
	reg_w16(OP2, TMP);
	next(5);
}

void mcs96_device::sub_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP = do_sub(reg_r16(OP2), TMP);
	reg_w16(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::sub_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP = do_sub(reg_r16(OP2), TMP);
	reg_w16(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::mulu_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP2 &= 0xfc;
	TMP = reg_r16(OP1);
	TMP *= reg_r16(OP2);
	reg_w16(OP2, TMP);
	reg_w16(OP2+2, TMP >> 16);
	next(25);
}

void mcs96_device::mulu_immed_2w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	OP2 &= 0xfc;
	TMP = OP1 * reg_r16(OP2);
	reg_w16(OP2, TMP);
	reg_w16(OP2+2, TMP >> 16);
	next(26);
}

void mcs96_device::mulu_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP2 &= 0xfc;
	TMP = any_r16(OP1);
	TMP *= reg_r16(OP2);
	reg_w16(OP2, TMP);
	reg_w16(OP2+2, TMP >> 16);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(28); // +4 when external
	} else {
		next(27); // +4 when external
	}
}

void mcs96_device::mulu_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP2 &= 0xfc;
	TMP = any_r16(OP1);
	TMP *= reg_r16(OP2);
	reg_w16(OP2, TMP);
	reg_w16(OP2+2, TMP >> 16);
	next(OPI & 0x01 ? 28 : 27);
}

void mcs96_device::mul_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP2 &= 0xfc;
	TMP = reg_r16(OP1);
	TMP = int16_t(reg_r16(OP2)) * int16_t(TMP);
	reg_w16(OP2, TMP);
	reg_w16(OP2+2, TMP >> 16);
	next(29);
}

void mcs96_device::mul_immed_2w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	OP2 &= 0xfc;
	TMP = int16_t(OP1) * int16_t(reg_r16(OP2));
	reg_w16(OP2, TMP);
	reg_w16(OP2+2, TMP >> 16);
	next(30);
}

void mcs96_device::mul_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP2 &= 0xfc;
	TMP = any_r16(OP1);
	TMP = int16_t(reg_r16(OP2)) * int16_t(TMP);
	reg_w16(OP2, TMP);
	reg_w16(OP2+2, TMP >> 16);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(32); // +4 when external
	} else {
		next(31); // +4 when external
	}
}

void mcs96_device::mul_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP2 &= 0xfc;
	TMP = any_r16(OP1);
	TMP = int16_t(reg_r16(OP2)) * int16_t(TMP);
	reg_w16(OP2, TMP);
	reg_w16(OP2+2, TMP >> 16);
	next(OPI & 0x01 ? 32 : 31);
}

void mcs96_device::andb_direct_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r8(OP1);
	TMP &= reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::andb_immed_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = OP1 & reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::andb_indirect_2b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP &= reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::andb_indexed_2b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP &= reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::addb_direct_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r8(OP1);
	TMP = do_addb(reg_r8(OP2), TMP);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::addb_immed_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = do_addb(reg_r8(OP2), OP1);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::addb_indirect_2b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP = do_addb(reg_r8(OP2), TMP);
	reg_w8(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::addb_indexed_2b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP = do_addb(reg_r8(OP2), TMP);
	reg_w8(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::subb_direct_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r8(OP1);
	TMP = do_subb(reg_r8(OP2), TMP);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::subb_immed_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = do_subb(reg_r8(OP2), OP1);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::subb_indirect_2b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP = do_subb(reg_r8(OP2), TMP);
	reg_w8(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::subb_indexed_2b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP = do_subb(reg_r8(OP2), TMP);
	reg_w8(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::mulub_direct_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP2 &= 0xfe;
	TMP = reg_r8(OP1);
	TMP *= reg_r8(OP2);
	reg_w16(OP2, TMP);
	next(17);
}

void mcs96_device::mulub_immed_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP2 &= 0xfe;
	TMP = OP1 * reg_r8(OP2);
	reg_w16(OP2, TMP);
	next(17);
}

void mcs96_device::mulub_indirect_2b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP2 &= 0xfe;
	TMP = any_r8(OP1);
	TMP *= reg_r8(OP2);
	reg_w16(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(20); // +4 when external
	} else {
		next(19); // +4 when external
	}
}

void mcs96_device::mulub_indexed_2b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP2 &= 0xfe;
	TMP = any_r8(OP1);
	TMP *= reg_r8(OP2);
	reg_w16(OP2, TMP);
	next(OPI & 0x01 ? 20 : 19);
}

void mcs96_device::mulb_direct_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP2 &= 0xfe;
	TMP = reg_r8(OP1);
	TMP = int8_t(reg_r16(OP2)) * int8_t(TMP);
	reg_w16(OP2, TMP);
	next(21);
}

void mcs96_device::mulb_immed_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP2 &= 0xfe;
	TMP = int8_t(OP1) * int8_t(reg_r8(OP2));
	reg_w16(OP2, TMP);
	next(21);
}

void mcs96_device::mulb_indirect_2b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP2 &= 0xfe;
	TMP = any_r8(OP1);
	TMP = int8_t(reg_r8(OP2)) * int8_t(TMP);
	reg_w16(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(24); // +4 when external
	} else {
		next(23); // +4 when external
	}
}

void mcs96_device::mulb_indexed_2b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP2 &= 0xfe;
	TMP = any_r16(OP1);
	TMP = int8_t(reg_r8(OP2)) * int8_t(TMP);
	reg_w16(OP2, TMP);
	next(OPI & 0x01 ? 24 : 23);
}

void mcs96_device::or_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r16(OP1);
	TMP |= reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP2, TMP);
	next(4);
}

void mcs96_device::or_immed_2w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	TMP = OP1 | reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP2, TMP);
	next(5);
}

void mcs96_device::or_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP |= reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::or_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP |= reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::xor_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r16(OP1);
	TMP ^= reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP2, TMP);
	next(4);
}

void mcs96_device::xor_immed_2w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	TMP = OP1 ^ reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP2, TMP);
	next(5);
}

void mcs96_device::xor_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP ^= reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::xor_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP ^= reg_r16(OP2);
	set_nz16(TMP);
	reg_w16(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::cmp_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r16(OP1);
	do_sub(reg_r16(OP2), TMP);
	next(4);
}

void mcs96_device::cmp_immed_2w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	do_sub(reg_r16(OP2), OP1);
	next(5);
}

void mcs96_device::cmp_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r16(OP1);
	do_sub(reg_r16(OP2), TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::cmp_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r16(OP1);
	do_sub(reg_r16(OP2), TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::divu_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP2 &= 0xfc;
	PSW &= ~F_V;
	OP1 = reg_r16(OP1);
	if(OP1) {
		TMP = reg_r16(OP2);
		TMP |= reg_r16(OP2+2) << 16;
		uint32_t TMP2 = TMP / OP1;
		if(TMP2 > 65535)
			PSW |= F_V|F_VT;
		TMP = TMP % OP1;
		TMP = (TMP2 & 0xffff) | ((TMP & 0xffff) << 16);
		reg_w16(OP2, TMP);
		reg_w16(OP2+2, TMP >> 16);
	}
	next(25);
}

void mcs96_device::divu_immed_2w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	OP2 &= 0xfc;
	PSW &= ~F_V;
	if(OP1) {
		TMP = reg_r16(OP2);
		TMP |= reg_r16(OP2+2) << 16;
		uint32_t TMP2 = TMP / OP1;
		if(TMP2 > 65535)
			PSW |= F_V|F_VT;
		TMP = TMP % OP1;
		TMP = (TMP2 & 0xffff) | ((TMP & 0xffff) << 16);
		reg_w16(OP2, TMP);
		reg_w16(OP2+2, TMP >> 16);
	}
	next(26);
}

void mcs96_device::divu_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP2 &= 0xfc;
	PSW &= ~F_V;
	uint32_t d = any_r16(OP1);
	if(d) {
		TMP = reg_r16(OP2);
		TMP |= reg_r16(OP2+2) << 16;
		uint32_t TMP2 = TMP / d;
		if(TMP2 > 65535)
			PSW |= F_V|F_VT;
		TMP = TMP % d;
		TMP = (TMP2 & 0xffff) | ((TMP & 0xffff) << 16);
		reg_w16(OP2, TMP);
		reg_w16(OP2+2, TMP >> 16);
	}
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(29); // +4 when external
	} else {
		next(28); // +4 when external
	}
}

void mcs96_device::divu_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP2 &= 0xfc;
	PSW &= ~F_V;
	uint32_t d = any_r16(OP1);
	if(d) {
		TMP = reg_r16(OP2);
		TMP |= reg_r16(OP2+2) << 16;
		uint32_t TMP2 = TMP / d;
		if(TMP2 > 65535)
			PSW |= F_V|F_VT;
		TMP = TMP % d;
		TMP = (TMP2 & 0xffff) | ((TMP & 0xffff) << 16);
		reg_w16(OP2, TMP);
		reg_w16(OP2+2, TMP >> 16);
	}
	next(OPI & 0x01 ? 29 : 28);
}

void mcs96_device::div_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	OP2 &= 0xfc;
	PSW &= ~F_V;
	OP1 = reg_r16(OP1);
	if(OP1) {
		TMP = reg_r16(OP2);
		TMP |= reg_r16(OP2+2) << 16;
		int32_t TMP2 = int32_t(TMP) / int16_t(OP1);
		if(TMP2 > 32767 || TMP2 < -32768)
			PSW |= F_V|F_VT;
		TMP = TMP % int16_t(OP1);
		TMP = (TMP2 & 0xffff) | ((TMP & 0xffff) << 16);
		reg_w16(OP2, TMP);
		reg_w16(OP2+2, TMP >> 16);
	}
	next(30);
}

void mcs96_device::div_immed_2w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	OP2 &= 0xfc;
	PSW &= ~F_V;
	if(OP1) {
		TMP = reg_r16(OP2);
		TMP |= reg_r16(OP2+2) << 16;
		int32_t TMP2 = int32_t(TMP) / int16_t(OP1);
		if(TMP2 > 32767 || TMP2 < -32768)
			PSW |= F_V|F_VT;
		TMP = TMP % int16_t(OP1);
		TMP = (TMP2 & 0xffff) | ((TMP & 0xffff) << 16);
		reg_w16(OP2, TMP);
		reg_w16(OP2+2, TMP >> 16);
	}
	next(30);
}

void mcs96_device::div_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	OP2 &= 0xfc;
	PSW &= ~F_V;
	int32_t d = int16_t(any_r16(OP1));
	if(d) {
		TMP = reg_r16(OP2);
		TMP |= reg_r16(OP2+2) << 16;
		int32_t TMP2 = int32_t(TMP) / d;
		if(TMP2 > 32767 || TMP2 < -32768)
			PSW |= F_V|F_VT;
		TMP = TMP % d;
		TMP = (TMP2 & 0xffff) | ((TMP & 0xffff) << 16);
		reg_w16(OP2, TMP);
		reg_w16(OP2+2, TMP >> 16);
	}
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(33); // +4 when external
	} else {
		next(32); // +4 when external
	}
}

void mcs96_device::div_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	OP2 &= 0xfc;
	PSW &= ~F_V;
	int32_t d = int16_t(any_r16(OP1));
	if(d) {
		TMP = reg_r16(OP2);
		TMP |= reg_r16(OP2+2) << 16;
		int32_t TMP2 = int32_t(TMP) / d;
		if(TMP2 > 32767 || TMP2 < -32768)
			PSW |= F_V|F_VT;
		TMP = TMP % d;
		TMP = (TMP2 & 0xffff) | ((TMP & 0xffff) << 16);
		reg_w16(OP2, TMP);
		reg_w16(OP2+2, TMP >> 16);
	}
	next(OPI & 0x01 ? 33 : 32);
}

void mcs96_device::orb_direct_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r8(OP1);
	TMP |= reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::orb_immed_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = OP1 | reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::orb_indirect_2b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP |= reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::orb_indexed_2b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP |= reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::xorb_direct_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r8(OP1);
	TMP ^= reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::xorb_immed_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = OP1 ^ reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::xorb_indirect_2b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP ^= reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::xorb_indexed_2b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP ^= reg_r8(OP2);
	set_nz8(TMP);
	reg_w8(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::cmpb_direct_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r8(OP1);
	do_subb(reg_r8(OP2), TMP);
	next(4);
}

void mcs96_device::cmpb_immed_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	do_subb(reg_r8(OP2), OP1);
	next(4);
}

void mcs96_device::cmpb_indirect_2b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r8(OP1);
	do_subb(reg_r8(OP2), TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::cmpb_indexed_2b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r8(OP1);
	do_subb(reg_r8(OP2), TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::divub_direct_2e_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	PSW &= ~F_V;
	OP1 = reg_r8(OP1);
	if(OP1) {
		TMP = reg_r16(OP2);
		uint32_t TMP2 = TMP / OP1;
		if(TMP2 > 255)
			PSW |= F_V|F_VT;
		TMP = TMP % OP1;
		TMP = (TMP2 & 0xff) | ((TMP & 0xff) << 8);
		reg_w16(OP2, TMP);
	}
	next(17);
}

void mcs96_device::divub_immed_2e_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	PSW &= ~F_V;
	if(OP1) {
		TMP = reg_r16(OP2);
		uint32_t TMP2 = TMP / OP1;
		if(TMP2 > 255)
			PSW |= F_V|F_VT;
		TMP = TMP % OP1;
		TMP = (TMP2 & 0xff) | ((TMP & 0xff) << 8);
		reg_w16(OP2, TMP);
	}
	next(17);
}

void mcs96_device::divub_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	PSW &= ~F_V;
	uint32_t d = any_r8(OP1);
	if(d) {
		TMP = reg_r16(OP2);
		uint32_t TMP2 = TMP / d;
		if(TMP2 > 255)
			PSW |= F_V|F_VT;
		TMP = TMP % d;
		TMP = (TMP2 & 0xff) | ((TMP & 0xff) << 8);
		reg_w16(OP2, TMP);
	}
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(21); // +4 when external
	} else {
		next(20); // +4 when external
	}
}

void mcs96_device::divub_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	PSW &= ~F_V;
	uint32_t d = any_r8(OP1);
	if(d) {
		TMP = reg_r16(OP2);
		uint32_t TMP2 = TMP / d;
		if(TMP2 > 255)
			PSW |= F_V|F_VT;
		TMP = TMP % d;
		TMP = (TMP2 & 0xff) | ((TMP & 0xff) << 8);
		reg_w16(OP2, TMP);
	}
	next(OPI & 0x01 ? 21 : 20);
}

void mcs96_device::divb_direct_2e_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	PSW &= ~F_V;
	OP1 = reg_r8(OP1);
	if(OP1) {
		TMP = reg_r16(OP2);
		uint32_t TMP2 = int16_t(TMP) / int8_t(OP1);
		if(int16_t(TMP2) > 127 || int16_t(TMP2) < -128)
			PSW |= F_V|F_VT;
		TMP = int16_t(TMP) % int8_t(OP1);
		TMP = (TMP2 & 0xff) | ((TMP & 0xff) << 8);
		reg_w16(OP2, TMP);
	}
	next(21);
}

void mcs96_device::divb_immed_2e_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	PSW &= ~F_V;
	if(OP1) {
		TMP = reg_r16(OP2);
		uint32_t TMP2 = int16_t(TMP) / int8_t(OP1);
		if(int16_t(TMP2) > 127 || int16_t(TMP2) < -128)
			PSW |= F_V|F_VT;
		TMP = int16_t(TMP) % int8_t(OP1);
		TMP = (TMP2 & 0xff) | ((TMP & 0xff) << 8);
		reg_w16(OP2, TMP);
	}
	next(21);
}

void mcs96_device::divb_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	PSW &= ~F_V;
	int32_t d = int8_t(any_r8(OP1));
	if(d) {
		TMP = reg_r16(OP2);
		uint32_t TMP2 = int16_t(TMP) / d;
		if(int16_t(TMP2) > 127 || int16_t(TMP2) < -128)
			PSW |= F_V|F_VT;
		TMP = int16_t(TMP) % d;
		TMP = (TMP2 & 0xff) | ((TMP & 0xff) << 8);
		reg_w16(OP2, TMP);
	}
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(25); // +4 when external
	} else {
		next(24); // +4 when external
	}
}

void mcs96_device::divb_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	PSW &= ~F_V;
	int32_t d = int8_t(any_r8(OP1));
	if(d) {
		TMP = reg_r16(OP2);
		uint32_t TMP2 = int16_t(TMP) / d;
		if(int16_t(TMP2) > 127 || int16_t(TMP2) < -128)
			PSW |= F_V|F_VT;
		TMP = int16_t(TMP) % d;
		TMP = (TMP2 & 0xff) | ((TMP & 0xff) << 8);
		reg_w16(OP2, TMP);
	}
	next(OPI & 0x01 ? 24 : 1);
}

void mcs96_device::ld_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	reg_w16(OP2, reg_r16(OP1));
	next(4);
}

void mcs96_device::ld_immed_2w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	reg_w16(OP2, OP1);
	next(5);
}

void mcs96_device::ld_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	reg_w16(OP2, any_r16(OP1));
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::ld_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	reg_w16(OP2, any_r16(OP1));
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::addc_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r16(OP1);
	TMP = do_addc(reg_r16(OP2), TMP);
	reg_w16(OP2, TMP);
	next(4);
}

void mcs96_device::addc_immed_2w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	TMP = do_addc(reg_r16(OP2), OP1);
	reg_w16(OP2, TMP);
	next(5);
}

void mcs96_device::addc_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP = do_addc(reg_r16(OP2), TMP);
	reg_w16(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::addc_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP = do_addc(reg_r16(OP2), TMP);
	reg_w16(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::subc_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r16(OP1);
	TMP = do_subc(reg_r16(OP2), TMP);
	reg_w16(OP2, TMP);
	next(4);
}

void mcs96_device::subc_immed_2w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	OP2 = read_pc();
	TMP = do_subc(reg_r16(OP2), OP1);
	reg_w16(OP2, TMP);
	next(5);
}

void mcs96_device::subc_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP = do_subc(reg_r16(OP2), TMP);
	reg_w16(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::subc_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r16(OP1);
	TMP = do_subc(reg_r16(OP2), TMP);
	reg_w16(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::ldbze_direct_2e_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	reg_w16(OP2, uint8_t(reg_r8(OP1)));
	next(4);
}

void mcs96_device::ldbze_immed_2e_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	reg_w16(OP2, uint8_t(OP1));
	next(4);
}

void mcs96_device::ldbze_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	reg_w16(OP2, uint8_t(any_r8(OP1)));
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::ldbze_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	reg_w16(OP2, uint8_t(any_r8(OP1)));
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::ldb_direct_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	reg_w8(OP2, reg_r8(OP1));
	next(4);
}

void mcs96_device::ldb_immed_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	reg_w8(OP2, OP1);
	next(4);
}

void mcs96_device::ldb_indirect_2b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	reg_w8(OP2, any_r8(OP1));
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::ldb_indexed_2b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	reg_w8(OP2, any_r8(OP1));
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::addcb_direct_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r8(OP1);
	TMP = do_addcb(reg_r8(OP2), TMP);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::addcb_immed_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = do_addcb(reg_r8(OP2), OP1);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::addcb_indirect_2b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP = do_addcb(reg_r8(OP2), TMP);
	reg_w8(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::addcb_indexed_2b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP = do_addcb(reg_r8(OP2), TMP);
	reg_w8(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::subcb_direct_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = reg_r8(OP1);
	TMP = do_subcb(reg_r8(OP2), TMP);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::subcb_immed_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	TMP = do_subcb(reg_r8(OP2), OP1);
	reg_w8(OP2, TMP);
	next(4);
}

void mcs96_device::subcb_indirect_2b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP = do_subcb(reg_r8(OP2), TMP);
	reg_w8(OP2, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::subcb_indexed_2b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	TMP = any_r8(OP1);
	TMP = do_subcb(reg_r8(OP2), TMP);
	reg_w8(OP2, TMP);
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::ldbse_direct_2e_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	reg_w16(OP2, int8_t(reg_r8(OP1)));
	next(4);
}

void mcs96_device::ldbse_immed_2e_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	reg_w16(OP2, int8_t(OP1));
	next(4);
}

void mcs96_device::ldbse_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	reg_w16(OP2, int8_t(any_r8(OP1)));
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(7); // +4 when external
	} else {
		next(6); // +4 when external
	}
}

void mcs96_device::ldbse_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	reg_w16(OP2, int8_t(any_r8(OP1)));
	next(OPI & 0x01 ? 7 : 6);
}

void mcs96_device::st_direct_2w_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	reg_w16(OP1, reg_r16(OP2));
	next(4);
}

void mcs96_device::st_indirect_2w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	any_w16(OP1, reg_r16(OP2));
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(8); // +4 when external
	} else {
		next(7); // +4 when external
	}
}

void mcs96_device::st_indexed_2w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	any_w16(OP1, reg_r16(OP2));
	next(OPI & 0x01 ? 8 : 7);
}

void mcs96_device::stb_direct_2b_full()
{
	OP1 = read_pc();
	OP2 = read_pc();
	reg_w8(OP1, reg_r8(OP2));
	next(4);
}

void mcs96_device::stb_indirect_2b_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	OP2 = read_pc();
	any_w8(OP1, reg_r8(OP2));
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 1);
		next(8); // +4 when external
	} else {
		next(7); // +4 when external
	}
}

void mcs96_device::stb_indexed_2b_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else if(OP1 & 0x80)
		OP1 |= 0xff00;
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	OP2 = read_pc();
	if (OP1 == 0x380)
		OP1 += 0;
	any_w8(OP1, reg_r8(OP2));
	next(OPI & 0x01 ? 8 : 7);
}

void mcs96_device::push_direct_1w_full()
{
	OP1 = read_pc();
	TMP = reg_r16(0x18);
	TMP -= 2;
	reg_w16(0x18, TMP);
	OP1 = reg_r16(OP1);
	any_w16(TMP, OP1);
	next(8); // +4 is external sp
}

void mcs96_device::push_immed_1w_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	TMP = reg_r16(0x18);
	TMP -= 2;
	reg_w16(0x18, TMP);
	any_w16(TMP, OP1);
	next(8); // +4 is external sp
}

void mcs96_device::push_indirect_1w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	TMP = reg_r16(0x18);
	TMP -= 2;
	reg_w16(0x18, TMP);
	OP1 = any_r16(OP1);
	any_w16(TMP, OP1);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(12); // +4 when external
	} else {
		next(11); // +4 when external
	}
}

void mcs96_device::push_indexed_1w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else
		OP1 = int8_t(OP1);
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	TMP = reg_r16(0x18);
	TMP -= 2;
	reg_w16(0x18, TMP);
	OP1 = any_r16(OP1);
	any_w16(TMP, OP1);
	next(OPI & 0x01 ? 12 : 11);
}

void mcs96_device::pop_direct_1w_full()
{
	OP1 = read_pc();
	TMP = reg_r16(0x18);
	reg_w16(0x18, TMP+2);
	TMP = any_r16(TMP);
	reg_w16(OP1, TMP);
	next(12); // +2 when external sp
}

void mcs96_device::pop_indirect_1w_full()
{
	OPI = read_pc();
	OP1 = reg_r16(OPI);
	TMP = reg_r16(0x18);
	reg_w16(0x18, TMP+2);
	TMP = any_r16(TMP);
	if((OPI & 0xfe) == 0x18)
		OP1 += 2;
	any_w16(OP1, TMP);
	if(OPI & 0x01) {
		reg_w16(OPI, OP1 + 2);
		next(14); // +4 when external
	} else {
		next(14); // +4 when external
	}
}

void mcs96_device::pop_indexed_1w_full()
{
	OPI = read_pc();
	OP1 = read_pc();
	if(OPI & 0x01) {
		OPI &= 0xfe;
		OP1 |= read_pc() << 8;
	} else
		OP1 = int8_t(OP1);
	if(OPI) {
		OP1 += reg_r16(OPI);
	}
	TMP = reg_r16(0x18);
	reg_w16(0x18, TMP+2);
	TMP = any_r16(TMP);
	any_w16(OP1, TMP);
	next(OPI & 0x01 ? 14 : 14);
}

void mcs96_device::jnst_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(!(PSW & F_ST)) {
		PC += OP1;
		next(8);
	} else {
		PSW &= ~F_VT;
		next(4);
	}
}

void mcs96_device::jnh_rel8_full()
{
	OP1 = int8_t(read_pc());
	if((PSW & (F_C|F_Z)) != F_C) {
		PC += OP1;
		next(8);
	} else {
		next(4);
	}
}

void mcs96_device::jgt_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(!(PSW & (F_Z|F_N))) {
		PC += OP1;
		next(8);
	} else {
		next(4);
	}
}

void mcs96_device::jnc_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(!(PSW & F_C)) {
		PC += OP1;
		next(8);
	} else {
		next(4);
	}
}

void mcs96_device::jnvt_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(!(PSW & F_VT)) {
		PC += OP1;
		next(8);
	} else {
		PSW &= ~F_VT;
		next(4);
	}
}

void mcs96_device::jnv_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(!(PSW & F_V)) {
		PC += OP1;
		next(8);
	} else {
		next(4);
	}
}

void mcs96_device::jge_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(!(PSW & F_N)) {
		PC += OP1;
		next(8);
	} else {
		next(4);
	}
}

void mcs96_device::jne_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(!(PSW & F_Z)) {
		PC += OP1;
		next(8);
	} else {
		next(4);
	}
}

void mcs96_device::jst_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(PSW & F_ST) {
		PC += OP1;
		next(8);
	} else {
		PSW &= ~F_VT;
		next(4);
	}
}

void mcs96_device::jh_rel8_full()
{
	OP1 = int8_t(read_pc());
	if((PSW & (F_C|F_Z)) == F_C) {
		PC += OP1;
		next(8);
	} else {
		next(4);
	}
}

void mcs96_device::jle_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(PSW & (F_Z|F_N)) {
		PC += OP1;
		next(8);
	} else {
		next(4);
	}
}

void mcs96_device::jc_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(PSW & F_C) {
		PC += OP1;
		next(8);
	} else {
		next(4);
	}
}

void mcs96_device::jvt_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(PSW & F_VT) {
		PSW &= ~F_VT;
		PC += OP1;
		next(8);
	} else {
		next(4);
	}
}

void mcs96_device::jv_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(PSW & F_V) {
		PC += OP1;
		next(8);
	} else {
		next(4);
	}
}

void mcs96_device::jlt_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(PSW & F_N) {
		PC += OP1;
		next(8);
	} else {
		next(4);
	}
}

void mcs96_device::je_rel8_full()
{
	OP1 = int8_t(read_pc());
	if(PSW & F_Z) {
		PC += OP1;
		next(8);
	} else {
		next(4);
	}
}

void mcs96_device::djnz_rrel8_full()
{
	OP2 = read_pc();
	OP1 = int8_t(read_pc());
	TMP = reg_r8(OP2);
	TMP = uint8_t(TMP-1);
	reg_w8(OP2, TMP);
	if(TMP) {
		PC += OP1;
		next(9);
	} else {
		next(5);
	}
}

void mcs96_device::br_indirect_1n_full()
{
	OPI = read_pc() & 0xfe;
	OP1 = reg_r16(OPI);
	PC = OP1;
	next(8);
}

void mcs96_device::ljmp_rel16_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	PC += OP1;
	next(8);
}

void mcs96_device::lcall_rel16_full()
{
	OP1 = read_pc();
	OP1 |= read_pc() << 8;
	TMP = reg_r16(0x18);
	TMP -= 2;
	reg_w16(0x18, TMP);
	any_w16(TMP, PC);
	PC += OP1;
	next(13); // +3 for external sp
}

void mcs96_device::ret_none_full()
{
	TMP = reg_r16(0x18);
	reg_w16(0x18, TMP+2);
	PC = any_r16(TMP);
	next(12); // +4 for external sp
}

void mcs96_device::pushf_none_full()
{
	TMP = reg_r16(0x18);
	TMP -= 2;
	reg_w16(0x18, TMP);
	any_w16(TMP, PSW);
	PSW = 0x0000;
	check_irq();
	next_noirq(8); // +4 for external sp
}

void mcs96_device::popf_none_full()
{
	TMP = reg_r16(0x18);
	reg_w16(0x18, TMP+2);
	PSW = any_r16(TMP);
	check_irq();
	next_noirq(9); // +4 for external sp
}

void mcs96_device::trap_none_full()
{
	TMP = reg_r16(0x18);
	TMP -= 2;
	reg_w16(0x18, TMP);
	any_w16(TMP, PC);
	PC  = any_r16(0x2010);
	next_noirq(21); // +3 for external sp
}

void mcs96_device::clrc_none_full()
{
	PSW &= ~F_C;
	next(4);
}

void mcs96_device::setc_none_full()
{
	PSW |= F_C;
	next(4);
}

void mcs96_device::di_none_full()
{
	PSW &= ~F_I;
	check_irq();
	next_noirq(4);
}

void mcs96_device::ei_none_full()
{
	PSW |= F_I;
	check_irq();
	next_noirq(4);
}

void mcs96_device::clrvt_none_full()
{
	PSW &= ~F_VT;
	next(4);
}

void mcs96_device::nop_none_full()
{
	next(4);
}

void mcs96_device::rst_none_full()
{
	PC = 0x2080;
	next(4);
}

void mcs96_device::fetch_full()
{
	if(irq_requested) {
		int level;
		for(level = 7; level >= 0 && !(PSW & pending_irq & (1<<level)); level--);
		//if(level != 7)
		if (level >= 0)
			pending_irq &= ~(1<<level);
		OP1 = level;
		//standard_irq_callback(OP1, PC);
		TMP = reg_r16(0x18);
		TMP -= 2;
		reg_w16(0x18, TMP);
		any_w16(TMP, PC);
		PC = any_r16(0x2000+2*OP1);
		check_irq();
	}
	PPC = PC;
	//if(machine().debug_flags & DEBUG_FLAG_ENABLED)
	//	debugger_instruction_hook(PC);
	OP1 = read_pc();
	if(OP1 == 0xfe) {
		OP1 = read_pc();
		inst_state = 0x100 | OP1;
	} else
		inst_state = OP1;
}

void mcs96_device::fetch_noirq_full()
{
	PPC = PC;
	//if(machine().debug_flags & DEBUG_FLAG_ENABLED)
	//	debugger_instruction_hook(PC);
	OP1 = read_pc();
	if(OP1 == 0xfe) {
		OP1 = read_pc();
		inst_state = 0x100 | OP1;
	} else
		inst_state = OP1;
}

