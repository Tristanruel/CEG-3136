  AREA    |.text|, CODE, READONLY
  PRESERVE8;
  THUMB;


  EXPORT update_points_s

update_points_s FUNCTION	; char update_points_s(player_t *player, uint8_t target_info);
	PUSH	{r4-r7,lr}	
	MOV r2, r0		
	MOVS r3, #0x00	
    LDRH r0,[r2,#0x00]
	ADDS r0,r0,#1	
    STRH r0,[r2,#0x00]
	AND  r0,r1,#0xFC	
	CBZ  r0, endreturn	
	AND r0, r1, #0x02
	CBNZ r0, endreturn	
	LDRH r5,[r2,#0x02]	
	ADDS r5,r5,#1	
    STRH r5,[r2,#0x02]	
	UBFX r0, r1, #2, #3 
	UBFX r4, r1, #5, #2 
    SUBS r5,r0,#1	
	ADDS r6,r2,#6		
	ADD r5,r6,r5,LSL #2	
	LDRB r5,[r5,r4]		
	ADDS r5,r5,#1	
	UXTB r7,r5	
	SUBS r5,r0,#1
	ADD r5,r6,r5,LSL #2		
	STRB r7,[r5,r4]		
	ADDS r3,r3, #1	
	SUBS r5, r0, #1		
	ADD r5, r6, r5, LSL #2
	LDRB r5, [r5,r4]	
	CMP r5, r0		
	BNE endreturn
	ADD r3, r3, r0		
endreturn	LDRH r0, [r2,#0x04]	
	ADD r0, r0, r3			
	STRH r0, [r2,#0x04]		
	MOV r0, r3				
	POP {r4-r7,pc}
	BX	LR					
  ENDFUNC
  
  EXPORT player_get_num_pieces_destroyed_s

player_get_num_pieces_destroyed_s FUNCTION	; int  player_get_num_pieces_destroyed_s (player_t * player);
	PUSH {r4-r5,lr}			
	MOV R0, R3			
	MOV R0, #0		
	MOV R1, #0			
	MOV R2, #0			
loop1	CMP R1, #0x04	
		BGE endloop1		

loop2	CMP R2, #0x04	
			BGE endloop2	
			ADDS r4, r3, #0x06		
			ADD r4, r4, r1, LSL #0x02	
			LDRB r5, [r4, r2]	
			ADDS r4, r1, #0x01	
			CMP r5, r4	
			BNE incnot	
			ADDS r0, #0x01	
incnot		ADDS R2, #0x01
			B loop2	
endloop2	ADDS R1, #0x01		
			MOV r2, #0x00	
			B loop1			
endloop1	POP{r4-r5, pc}
			BX	LR				
  ENDFUNC
player_done_s FUNCTION	; char player_done_s(player_t * player);
	EXPORT player_done_s
		
	PUSH          {r4-r5,lr}
	MOV           r3,r0
	MOVS          r0,#0x01
	MOVS          r1,#0x00
	B             adress1
adress5	MOVS          r2,#0x00
	B             adress2;
adress4	ADDS          r4,r3,#6
	ADD           r4,r4,r1,LSL #2
	LDRB          r5,[r4,r2]
	ADDS          r4,r1,#1
	CMP           r5,r4
	BGE           adress3
	MOVS          r0,#0x00
	B             adress6
adress3	ADDS          r2,r2,#1
adress2	RSB           r4,r1,#0x04
	CMP           r4,r2
	BGT           adress4
adress6	NOP          
	ADDS          r1,r1,#1
adress1	CMP           r1,#0x04
	BLT           adress5
	POP			 {r4-r5,pc}
  ENDFUNC
  
  END