.686
.MODEL flat, C

?OnGameTick@OnTickHandler@LuaHookNG@@CIXPAUlua_State@2@PBD@Z PROTO SYSCALL
?SetFieldHandler@OnNewStateHandler@LuaHookNG@@CIXPAUlua_State@2@HPBD@Z PROTO SYSCALL

.DATA
	onGameTickTrampoline	dd 0

.CODE
	?SetOnGameTickTrampoline_ASM@OnTickHandler@LuaHookNG@@CAXPAX@Z PROC SYSCALL
		mov eax, [esp + 4]
		mov onGameTickTrampoline, eax
		retn
	?SetOnGameTickTrampoline_ASM@OnTickHandler@LuaHookNG@@CAXPAX@Z ENDP

	?OnGameTick_ASM@OnTickHandler@LuaHookNG@@CAXXZ PROC SYSCALL
		mov edx, [esp + 8]
		push ecx
		mov ecx, [ecx]
		call ?OnGameTick@OnTickHandler@LuaHookNG@@CIXPAUlua_State@2@PBD@Z
		pop ecx
		mov eax, [onGameTickTrampoline]
		jmp eax
	?OnGameTick_ASM@OnTickHandler@LuaHookNG@@CAXXZ ENDP

	?SetFieldHandler_ASM@OnNewStateHandler@LuaHookNG@@CAXXZ PROC SYSCALL
		pop eax
		mov ecx, [esp]
		mov edx, [esp + 4]
		push [esp + 8]
		push eax
		jmp ?SetFieldHandler@OnNewStateHandler@LuaHookNG@@CIXPAUlua_State@2@HPBD@Z
	?SetFieldHandler_ASM@OnNewStateHandler@LuaHookNG@@CAXXZ ENDP
END
