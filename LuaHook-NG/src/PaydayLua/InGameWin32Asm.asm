.686
.MODEL flat, C
?tGameTick@InGameHandling@Olipro@@0P6IPAXPAPAUlua_State@@HPAXPBD@ZA PROTO SYSCALL
?OnGameTick@InGameHandling@Olipro@@CIXPAUlua_State@@PBD@Z PROTO SYSCALL
?OnLuaSetField@InGameHandling@Olipro@@CIXPAUlua_State@@HPBD@Z PROTO SYSCALL

.CODE
	?HookDoGameTick@InGameHandling@Olipro@@CIPAXPAPAUlua_State@@HPAXPBD@Z PROC SYSCALL
		mov edx, [esp+8]
		push ecx
		mov ecx, [ecx]
		call ?OnGameTick@InGameHandling@Olipro@@CIXPAUlua_State@@PBD@Z
		pop ecx
		mov eax, ?tGameTick@InGameHandling@Olipro@@0P6IPAXPAPAUlua_State@@HPAXPBD@ZA
		mov eax, [eax]
		jmp eax
		retn
	?HookDoGameTick@InGameHandling@Olipro@@CIPAXPAPAUlua_State@@HPAXPBD@Z ENDP

	?HookLuaSetField@InGameHandling@Olipro@@CIXPAUlua_State@@HPBD@Z PROC SYSCALL
		pop eax
		push [esp]
		push eax
		jmp ?OnLuaSetField@InGameHandling@Olipro@@CIXPAUlua_State@@HPBD@Z
	?HookLuaSetField@InGameHandling@Olipro@@CIXPAUlua_State@@HPBD@Z ENDP
END
