.686
.MODEL flat, C

.CODE
	?SaveEsp@LuaHookNG@@YIXPAI@Z PROC SYSCALL
		lea eax, [esp+4]
		mov [ecx], eax
		retn
	?SaveEsp@LuaHookNG@@YIXPAI@Z ENDP

	?LoadEsp@LuaHookNG@@YIXI@Z PROC SYSCALL
		mov eax, [esp]
		mov esp, ecx
		jmp eax
	?LoadEsp@LuaHookNG@@YIXI@Z ENDP

END
