{
	if ($2 == "PATCH" && strtonum($1) > strtonum(0x900000))
		print ".text." $4 " " $1 " : { KEEP(" $3 " (.*." $4 ")) }";
	else if ($2 == "DUMMY" && strtonum($1) > strtonum(0x900000))
		print ".text.dummy." $4 " " $1 " : { " $3 " (.*." $4 ") }";
	else if ($2 == "REGION" && $1 != "fwpatch")
		print ".text." $4 " : { KEEP(" $3 " (.*." $4 ")) } >" $1;
	else if ($2 == "TARGETREGION" && $1 != "fwpatch")
		print ".text." $1 " : { " $3 " (.text .text.* .data .data.* .bss .bss.* .rodata .rodata.*) } >"  $1;
}
