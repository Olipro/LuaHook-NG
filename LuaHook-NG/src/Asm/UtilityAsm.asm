.686
.MODEL flat, C

.CODE
	?SaveEsp@Olipro@@YIXPAI@Z PROC SYSCALL
		lea eax, [esp+4]
		mov [ecx], eax
		retn
	?SaveEsp@Olipro@@YIXPAI@Z ENDP

	?LoadEsp@Olipro@@YIXI@Z PROC SYSCALL
		mov eax, [esp]
		mov esp, ecx
		jmp eax
	?LoadEsp@Olipro@@YIXI@Z ENDP

END
