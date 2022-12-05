[Command]
name = "AI1"
command = D,D,D,F,F,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI2"
command = D,D,D,F,F,U,a+b+c+x+y+z
time = 1

[Command]
name = "AI3"
command = D,D,D,F,F,UF,a+b+c+x+y+z
time = 1

[Command]
name = "AI4"
command = D,D,D,F,F,D,a+b+c+x+y+z
time = 1

[Command]
name = "AI5"
command = D,D,D,F,F,DF,a+b+c+x+y+z
time = 1

[Command]
name = "AI6"
command = D,D,D,F,F,B,a+b+c+x+y+z
time = 1

[Command]
name = "AI7"
command = D,D,D,F,F,DB,a+b+c+x+y+z
time = 1

[Command]
name = "AI8"
command = D,D,D,F,F,UB,a+b+c+x+y+z
time = 1

[Command]
name = "AI9"
command = D,D,D,F,U,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI10"
command = D,D,D,F,UF,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI11"
command = D,D,D,F,DF,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI12"
command = D,D,D,F,D,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI13"
command = D,D,D,F,DB,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI14"
command = D,D,D,F,B,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI15"
command = D,D,D,F,UB,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI16"
command = D,D,D,F,F,F,a+b+c+x+y,z
time = 1

[Command]
name = "AI17"
command = D,D,D,F,F,F,a+b+c+x,y,z
time = 1

[Command]
name = "AI18"
command = D,D,D,F,F,F,a+b+c,x,y,z
time = 1

[Command]
name = "AI19"
command = D,D,D,F,F,F,a+b,c,x,y,z
time = 1

[Command]
name = "AI20"
command = D,D,D,F,F,F,a,b,c,x,y,z
time = 1

[Command]
name = "AI21"
command = D,D,D,F,F,F,a+b+c,x+y+z
time = 1

[Command]
name = "AI22"
command = D,D,D,F,F,U,a+b,c+x+y+z
time = 1

[Command]
name = "AI23"
command = D,D,D,F,F,UF,a,b+c+x+y+z
time = 1

[Command]
name = "AI24"
command = D,D,D,F,F,U,a+b,c+x+y+z
time = 1

[Command]
name = "AI25"
command = D,D,D,F,F,DF,a,b,c+x+y+z
time = 1

[Command]
name = "AI26"
command = D,D,D,F,F,B,a+b,c+x,y+z
time = 1

[Command]
name = "AI27"
command = D,D,D,F,F,DB,a,b+c+x,y+z
time = 1

[Command]
name = "AI28"
command = D,D,D,F,F,UB,a,b+c+x+y,z
time = 1

[Command]
name = "AI29"
command = DF,D,D,F,U,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI30"
command = B,D,D,F,UF,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI31"
command = B,B,BD,F,DF,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI32"
command = D,F,F,F,D,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI33"
command = DF,D,DF,UF,DB,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI34"
command = D,D,D,F,DB,DF,a+b+c+x+y+z
time = 1

[Command]
name = "AI35"
command = D,D,UF,DF,UB,UF,a+b+c+x+y+z
time = 1

[Command]
name = "AI36"
command = D,DF,DF,DF,DF,F,a+b+c+x+y,z
time = 1

[Command]
name = "AI37"
command = D,D,DB,UF,DF,UF,a+b+c+x,y,z
time = 1

[Command]
name = "AI38"
command = F,U,F,F,F,F,a,b+c,x,y,z
time = 1

[Command]
name = "AI39"
command = DF,UF,D,DF,U,F,a+b,c+x,y,z
time = 1

[Command]
name = "AI40"
command = D,D,D,D,F,D,a+b,c,x,y+z
time = 1

[Remap]
x = x
y = y
z = z
a = a
b = b
c = c
s = s

;-| Default Values |-------------------------------------------------------
[Defaults]
; Default value for the "time" parameter of a Command. Minimum 1.
command.time = 30

; Default value for the "buffer.time" parameter of a Command. Minimum 1,
; maximum 30.
command.buffer.time = 1

;-| Super Motions |--------------------------------------------------------

[Command]
name = "sw"
command = ~D, DB, B,D, DB, B, a
time = 30

[Command]
name = "fire"
command = ~D, DB, B, a
time = 25

[Command]
name = "sha"
command = ~D, DF, F, b
time = 25

[Command]
name = "bun"
command = ~D, DB, B, b
time = 25

[Command]
name = "mizu"
command = ~D, DB, B,D, DB, B, b
time = 30

[Command]
name = "bun2"
command = ~D, DB, B, c
time = 25

[Command]
name = "bun3"
command = ~D, DB, B,D, DB, B, c
time = 30

[Command]
name = "fire2"
command = ~D, DB, B, x
time = 30

[Command]
name = "tele"
command = F, F, z
time = 30

[Command]
name = "kis"
command = ~D, DF, F, c
time = 30

[Command]
name = "kis2"
command = ~D, D, z
time = 30

[Command]
name = "crow"
command = ~D, DF, F,D, DF, F, c
time = 30

[Command]
name = "crow2"
command = ~D, D, y
time = 30

[Command]
name = "met"
command = ~D, DB, B,D, DB, B, x
time = 30

[Command]
name = "met2"
command = ~D, D, x
time = 30

;-| Special Motions |------------------------------------------------------

;-| Double Tap |-----------------------------------------------------------
[Command]
name = "FF"     ;Required (do not remove)
command = F, F
time = 10

[Command]
name = "BB"     ;Required (do not remove)
command = B, B
time = 10

;-| 2/3 Button Combination |-----------------------------------------------
[Command]
name = "recovery" ;Required (do not remove)
command = x+y
time = 1

[Command]
name = "recovery"
command = y+z
time = 1

[Command]
name = "recovery"
command = x+z
time = 1

[Command]
name = "recovery"
command = a+b
time = 1

[Command]
name = "recovery"
command = b+c
time = 1

[Command]
name = "recovery"
command = a+c
time = 1

;-| Dir + Button |---------------------------------------------------------
[Command]
name = "back_x"
command = /$B,x
time = 1

[Command]
name = "back_y"
command = /$B,y
time = 1

[Command]
name = "back_z"
command = /$B,z
time = 1

[Command]
name = "down_x"
command = /$D,x
time = 1

[Command]
name = "down_y"
command = /$D,y
time = 1

[Command]
name = "down_z"
command = /$D,z
time = 1

[Command]
name = "fwd_x"
command = /$F,x
time = 1

[Command]
name = "fwd_y"
command = /$F,y
time = 1

[Command]
name = "fwd_z"
command = /$F,z
time = 1

[Command]
name = "up_x"
command = /$U,x
time = 1

[Command]
name = "up_y"
command = /$U,y
time = 1

[Command]
name = "up_z"
command = /$U,z
time = 1

[Command]
name = "back_a"
command = /$B,a
time = 1

[Command]
name = "back_b"
command = /$B,b
time = 1

[Command]
name = "back_c"
command = /$B,c
time = 1

[Command]
name = "down_a"
command = /$D,a
time = 1

[Command]
name = "down_b"
command = /$D,b
time = 1

[Command]
name = "down_c"
command = /$D,c
time = 1

[Command]
name = "fwd_a"
command = /$F,a
time = 1

[Command]
name = "fwd_b"
command = /$F,b
time = 1

[Command]
name = "fwd_c"
command = /$F,c
time = 1

[Command]
name = "up_a"
command = /$U,a
time = 1

[Command]
name = "up_b"
command = /$U,b
time = 1

[Command]
name = "up_c"
command = /$U,c
time = 1

;-| Single Button |---------------------------------------------------------
[Command]
name = "a"
command = a
time = 1

[Command]
name = "b"
command = b
time = 1

[Command]
name = "c"
command = c
time = 1

[Command]
name = "x"
command = x
time = 1

[Command]
name = "y"
command = y
time = 1

[Command]
name = "z"
command = z
time = 1

[Command]
name = "s"
command = s
time = 1

;-| Single Dir |------------------------------------------------------------
[Command]
name = "fwd" ;Required (do not remove)
command = $F
time = 1

[Command]
name = "downfwd"
command = $DF
time = 1

[Command]
name = "down" ;Required (do not remove)
command = $D
time = 1

[Command]
name = "downback"
command = $DB
time = 1

[Command]
name = "back" ;Required (do not remove)
command = $B
time = 1

[Command]
name = "upback"
command = $UB
time = 1

[Command]
name = "up" ;Required (do not remove)
command = $U
time = 1

[Command]
name = "upfwd"
command = $UF
time = 1

;-| Hold Button |--------------------------------------------------------------
[Command]
name = "hold_x"
command = /x
time = 1

[Command]
name = "hold_y"
command = /y
time = 1

[Command]
name = "hold_z"
command = /z
time = 1

[Command]
name = "hold_a"
command = /a
time = 1

[Command]
name = "hold_b"
command = /b
time = 1

[Command]
name = "hold_c"
command = /c
time = 1

[Command]
name = "hold_s"
command = /s
time = 1

;-| Hold Dir |--------------------------------------------------------------
[Command]
name = "holdfwd" ;Required (do not remove)
command = /$F
time = 1

[Command]
name = "holddownfwd"
command = /$DF
time = 1

[Command]
name = "holddown" ;Required (do not remove)
command = /$D
time = 1

[Command]
name = "holddownback"
command = /$DB
time = 1

[Command]
name = "holdback" ;Required (do not remove)
command = /$B
time = 1

[Command]
name = "holdupback"
command = /$UB
time = 1

[Command]
name = "holdup" ;Required (do not remove)
command = /$U
time = 1

[Command]
name = "holdupfwd"
command = /$UF
time = 1

;---------------------------------------------------------------------------
; 2. State entry
; --------------
; This is where you define what commands bring you to what states.
;
; Each state entry block looks like:
;   [State -1, Label]           ;Change Label to any name you want to use to
;                               ;identify the state with.
;   type = ChangeState          ;Don't change this
;   value = new_state_number
;   trigger1 = command = command_name
;   . . .  (any additional triggers)
;
; - new_state_number is the number of the state to change to
; - command_name is the name of the command (from the section above)
; - Useful triggers to know:
;   - statetype
;       S, C or A : current state-type of player (stand, crouch, air)
;   - ctrl
;       0 or 1 : 1 if player has control. Unless "interrupting" another
;                move, you'll want ctrl = 1
;   - stateno
;       number of state player is in - useful for "move interrupts"
;   - movecontact
;       0 or 1 : 1 if player's last attack touched the opponent
;                useful for "move interrupts"
;
; Note: The order of state entry is important.
;   State entry with a certain command must come before another state
;   entry with a command that is the subset of the first.  
;   For example, command "fwd_a" must be listed before "a", and
;   "fwd_ab" should come before both of the others.
;
; For reference on triggers, see CNS documentation.
;
; Just for your information (skip if you're not interested):
; This part is an extension of the CNS. "State -1" is a special state
; that is executed once every game-tick, regardless of what other state
; you are in.


; Don't remove the following line. It's required by the CMD standard.
[Statedef -1]

;===========================================================================
;---------------------------------------------------------------------------
[State -1,Meteor]
type = changestate
value = 940
triggerall = power >= 3000
triggerall = var(39) = 0
triggerall = command = "met"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
[State -1,Crowz]
type = changestate
value = 909
triggerall = power >= 2000
triggerall = var(39) = 0
triggerall = command = "crow"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
[State -1,Kisame]
type = changestate
value = 899
triggerall = numhelper(900) = 0
triggerall = power >= 1000
triggerall = var(39) = 0
triggerall = command = "kis"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl

[State -1, AIR Fire]
type = changestate
value = 7802
triggerall = numhelper(7812) = 0
triggerall = power >= 1000
triggerall = var(39) = 0
triggerall = command = "fire2"
trigger1 = statetype = A
trigger1 = ctrl

[State -1, Fire]
type = changestate
value = 780
triggerall = numhelper(781) = 0
triggerall = power >= 1000
triggerall = var(39) = 0
triggerall = command = "fire2"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl


[State -1, Sharingan]
type = changestate
value = 789
triggerall = power >= 1000
triggerall = var(10) = 0
triggerall = var(39) = 0
triggerall = command = "sha"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl

[State -1,Mizu Bunshin]
type = changestate
value = 6542
triggerall = numhelper(16001) < 5
triggerall = power >= 1000
triggerall = var(39) = 0
triggerall = command = "mizu"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl

[State -1, Bunshin]
type = changestate
value = 654
triggerall = numhelper(15001) < 5
triggerall = power >= 1000
triggerall = var(39) = 0
triggerall = command = "bun"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl

[State -1, Bunshin Atk2]
type = changestate
value = 6552
triggerall = power >= 350
triggerall = var(39) = 0
triggerall = numhelper(656) < 5
triggerall = numhelper(15008) < 2
triggerall = command = "bun3"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl

[State -1, Bunshin Atk]
type = changestate
value = 655
triggerall = power >= 350
triggerall = var(39) = 0
triggerall = numhelper(656) < 5
triggerall = numhelper(15008) < 2
triggerall = command = "bun2"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl

; Swords
[State -1, Swords]
type = ChangeState
value = 845
triggerall = var(39) = 0
triggerall = power >= 3000 
triggerall = command = "sw"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl

; Fire
[State -1, Fire thingy]
type = ChangeState
value = 745
triggerall = var(39) = 0
triggerall = power >= 1500
triggerall = command = "fire"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
;-----------------------------------------------
; Stand Light Punch
[State -1, Crouch Tele]
type = ChangeState
value = 400
triggerall = command = "a"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
;===========================================================================
;---------------------------------------------------------------------------
; Run Fwd
[State -1, Run Fwd]
type = ChangeState
value = 100
trigger1 = command = "FF"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
; Run Back
[State -1, Run Back]
type = ChangeState
value = 105
trigger1 = command = "BB"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
; Throw
[State -1, Throw]
type = ChangeState
value = 800
triggerall = anim != 21
triggerall = stateno = 20
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
;-----------------------------------------------------------------------------
;[State -1, Counter]
;type = ChangeState
;value = 500
;triggerall = stateno != 120
;triggerall = stateno != 130
;triggerall = command = "x"
;trigger1 = movetype = H && (statetype = S)
;----------------------------------------------------------------------------
[State -1, Kavamari]
type = ChangeState
value = 501
triggerall = power >= 500
triggerall = stateno != 120
triggerall = stateno != 130
triggerall = stateno != 131
triggerall = stateno != 132
triggerall = stateno != 140
triggerall = stateno != 150
triggerall = stateno != 151
triggerall = stateno != 152
triggerall = stateno != 153
triggerall = stateno != 154
triggerall = stateno != 155
triggerall = p2stateno != 846
triggerall = command = "x"
trigger1 = movetype = H
trigger2 = stateno = 5030
trigger3 = stateno = 5035

[State -1,AIR counter]
type = ChangeState
value = 502
triggerall = stateno != 120
triggerall = stateno != 130
triggerall = stateno != 131
triggerall = stateno != 132
triggerall = stateno != 140
triggerall = stateno != 150
triggerall = stateno != 151
triggerall = stateno != 152
triggerall = stateno != 153
triggerall = stateno != 154
triggerall = stateno != 155
triggerall = p2stateno != 846
triggerall = statetype = A
triggerall = command = "b"
trigger1 = movetype = H
trigger2 = stateno = 5030
trigger3 = stateno = 5035
trigger4 = stateno = 5000
trigger5 = stateno = 5001



;===========================================================================
;---------------------------------------------------------------------------
; Stand Light Punch
[State -1, Chakra Charge]
type = ChangeState
value = 930
triggerall = command = "hold_a" && command = "hold_x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
; Stand Light Punch
[State -1, Stand Light Punch]
type = ChangeState
value = 200
triggerall = var(39) = 0
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 500 && animelemtime(7)>0

;---------------------------------------------------------------------------
; Stand Kunai stab
[State -1, Stand Kunai stab]
type = ChangeState
value = 210
triggerall = var(39) = 0
triggerall = command != "sha"
triggerall = command = "b"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 200 && animelemtime(3)>0
trigger3 = stateno = 500 && animelemtime(7)>0
;----------------------------------------------------------------------------
; Stand Heavy kick
[State -1, Stand Heavy kick]
type = ChangeState
value = 220
triggerall = var(39) = 0
triggerall = command = "c"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 210 && animelemtime(3)>0
trigger3 = stateno = 500 && animelemtime(7)>0
trigger4 = stateno = 200 && animelemtime(3)>0
;----------------------------------------------------------------------------
; Stand Uppercut
[State -1, Stand Uppercut]
type = ChangeState
value = 230
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 210 && animelemtime(4)>0
trigger3 = stateno = 200 && animelemtime(4)>0
trigger4 = stateno = 220 && animelemtime(5)>0
trigger5 = stateno = 500 && animelemtime(7)>0
;----------------------------------------------------------------------------
[State -1, Kunai throw]
type = ChangeState
value = 480
triggerall = numhelper(481) = 0
triggerall = command = "y"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
; Stand Light Punch
[State -1, Air Light Punch]
type = ChangeState
value = 600
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = A
trigger1 = ctrl

;---------------------------------------------------------------------------
; Stand Light Punch
[State -1, Air Hard Punch]
type = ChangeState
value = 610
triggerall = command = "y"
triggerall = command != "holddown"
trigger1 = statetype = A
trigger1 = ctrl
;---------------------------------------------------------------------------
[State -1, Button change]
type = ChangeState
value = 987
triggerall = var(39) = 0
triggerall = command = "s"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
[State -1, Button change 2]
type = ChangeState
value = 9872
triggerall = var(39) = 1
triggerall = command = "s"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
[State -1,Meteor]
type = changestate
value = 940
triggerall = power >= 3000
triggerall = var(39) = 1
triggerall = command = "met2"
;triggerall = command = "hold_x" && command = "hold_y"
trigger1 = ctrl
;---------------------------------------------------------------------------
[State -1,Crowz]
type = changestate
value = 909
triggerall = power >= 2000
triggerall = var(39) = 1
triggerall = command = "crow2"
;triggerall = command = "hold_x" && command = "hold_y"
trigger1 = ctrl
;---------------------------------------------------------------------------
[State -1,Kisame]
type = changestate
value = 899
triggerall = numhelper(900) = 0
triggerall = power >= 1000
triggerall = var(39) = 1
triggerall = command = "kis2"
;triggerall = command = "s"
;triggerall = command = "holddown"
;trigger1 = statetype = C
trigger1 = ctrl
;---------------------------------------------------------------------------
; Swords
[State -1, Swords]
type = ChangeState
value = 845
triggerall = var(39) = 1
triggerall = power >= 3000
triggerall = command = "c"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl

; Fire
[State -1, Fire thingy]
type = ChangeState
value = 745
triggerall = var(39) = 1
triggerall = power >= 1500
triggerall = command = "b"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl

[State -1, Sharingan]
type = changestate
value = 789
triggerall = power >= 1000
triggerall = var(10) = 0
triggerall = var(39) = 1
triggerall = command = "z"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl

[State -1, Bunshin]
type = changestate
value = 654
triggerall = power >= 1500
triggerall = numhelper(654) < 5
triggerall = var(39) = 1
triggerall = command = "z"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl

[State -1, Bunshin Atk]
type = changestate
value = 655
triggerall = power >= 350
triggerall = var(39) = 1
triggerall = numhelper(15008) = 0
triggerall = numhelper(656) < 5
triggerall = command = "b"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl


[State -1, Bunshin Atk2]
type = changestate
value = 6552
triggerall = power >= 350
triggerall = var(39) = 1
triggerall = numhelper(656) < 5
triggerall = numhelper(15008) < 2
triggerall = command = "c"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl

[State -1, Fire]
type = changestate
value = 780
triggerall = numhelper(781) = 0
triggerall = power >= 1000
triggerall = var(39) = 1
triggerall = command = "x"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl

[State -1,AIR Fire]
type = changestate
value = 7802
triggerall = numhelper(781) = 0
triggerall = power >= 1000
triggerall = var(39) = 1
triggerall = command = "x"
triggerall = command = "holddown"
trigger1 = statetype = A
trigger1 = ctrl

[State -1,Mizu Bunshin]
type = changestate
value = 6542
triggerall = numhelper(16001) < 5
triggerall = power >= 1500
triggerall = var(39) = 1
triggerall = command = "y"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
;===========================================================================
;---------------------------------------------------------------------------
;---------------------------------------------------------------------------
; Stand Light Punch
[State -1, Stand Light Punch]
type = ChangeState
value = 200
triggerall = command = "a"
triggerall = var(39) = 1
triggerall = command != "holddown"
triggerall = statetype = S
trigger1 = ctrl = 1
;---------------------------------------------------------------------------
; Stand Kunai stab
[State -1, Stand Kunai stab]
type = ChangeState
value = 210
triggerall = var(39) = 1
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl = 1
trigger2 = stateno = 200 && animelemtime(3)>0
;----------------------------------------------------------------------------
; Stand Heavy kick
[State -1, Stand Heavy kick]
type = ChangeState
value = 220
triggerall = var(39) = 1
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl = 1
trigger2 = stateno = 210 && animelemtime(3)>0

[State -1, AI ON]
type = VarSet
triggerall = var(20) != 1
trigger1 = command = "AI1"
trigger2 = command = "AI2"
trigger3 = command = "AI3"
trigger4 = command = "AI4"
trigger5 = command = "AI5"
trigger6 = command = "AI6"
trigger7 = command = "AI7"
trigger8 = command = "AI8"
trigger9 = command = "AI9"
trigger10 = command = "AI10"
trigger11 = command = "AI11"
trigger12 = command = "AI12"
trigger13 = command = "AI13"
trigger14 = command = "AI14"
trigger15 = command = "AI15"
trigger16 = command = "AI16"
trigger17 = command = "AI17"
trigger18 = command = "AI18"
trigger19 = command = "AI19"
trigger20 = command = "AI20"
trigger21 = command = "AI21"
trigger22 = command = "AI22"
trigger23 = command = "AI23"
trigger24 = command = "AI24"
trigger25 = command = "AI25"
trigger26 = command = "AI26"
trigger27 = command = "AI27"
trigger28 = command = "AI28"
trigger29 = command = "AI29"
trigger30 = command = "AI30"
trigger31 = command = "AI31"
trigger32 = command = "AI32"
trigger33 = command = "AI33"
trigger34 = command = "AI34"
trigger35 = command = "AI35"
trigger36 = command = "AI36"
trigger37 = command = "AI37"
trigger38 = command = "AI38"
trigger39 = command = "AI39"
trigger40 = command = "AI40"
var(20) = 1

; ==========================
[State -1,AI Standing Guard]
type = ChangeState
triggerall = var(20) = 1 ;AI trigger used
triggerall = Statetype != A ;Player is not in the air
triggerall = P2statetype != C ;Player is not crouching
triggerall = Statetype = S ;Player is currently standing
triggerall = P2Movetype = A ;Opponent is attacking
triggerall = Pos Y != [-1,-999]
triggerall = ctrl = 1
trigger1 = random <= 600 ;triggers at 80% probability
value = 130 ;Default standing guard state

; =============================
; AI Stand to Crouch Guard Transition
; =============================
[State -1,AI Stand to Crouch Guard Transition]
type = ChangeState
triggerall = var(20) = 1
triggerall = StateType != A
triggerall = P2statetype = C
triggerall = P2Movetype = A
triggerall = Pos Y != [-1,-999]
trigger1 = stateno = 150
trigger1 = 1
value = 152

; =============================
; AI Crouching Guard
; =============================
[State -1, AI Crouching Guard]
type = ChangeState
triggerall = var(20) = 1
triggerall = StateType != A
triggerall = P2statetype = C
triggerall = P2Movetype = A
triggerall = Pos Y != [-1,-999]
triggerall = ctrl = 1
trigger1 = random <= 600
value = 131

; =============================
; AI Crouch to Stand Guard Transition
; =============================
[State -1,AI Crouch to Stand Guard Transition]
type = ChangeState
triggerall = var(20) = 1
triggerall = Statetype != A
triggerall = P2statetype != C
triggerall = P2Movetype = A
trigger1 = 1
trigger1 = stateno = 152
value = 150

; =============================
; AI Aerial Guard
; =============================
[State -1,AI Aerial Guard]
type = ChangeState
triggerall = var(20) = 1
triggerall = Statetype = A
triggerall = P2Movetype = A
triggerall = ctrl = 1
trigger1 = random <= 600
value = 132
;---------------------------------------------------------------------

;light punch
[State -1, AI light punch]
type = ChangeState
triggerall = var(20) = 1
triggerall = random <= 50
triggerall = StateType != A
triggerall = Movetype != H
triggerall = P2Life > 0
triggerall = P2bodydist X <= 50
trigger1 = ctrl = 1
value = 200

;medium punch
[State -1, AI medium punch]
type = ChangeState
triggerall = Stateno = 200
trigger1 = movehit = 1
triggerall = random <= 30
triggerall = var(20) = 1
triggerall = StateType != A
triggerall = Movetype != H
triggerall = P2Life > 0
triggerall = P2bodydist X <= 50
value = 210

;heavy punch
[State -1, AI heavy punch]
type = ChangeState
triggerall = Stateno = 210
trigger1 = random <=10
trigger1 = movehit = 1
triggerall = var(20) = 1
triggerall = StateType != A
triggerall = Movetype != H
triggerall = P2Life > 0
triggerall = P2bodydist X <= 50
value = 220

;kawarmi
[State -1,AI kawarmi]
type = ChangeState
triggerall = power >= 500
triggerall = stateno != 120
triggerall = stateno != 130
triggerall = stateno != 131
triggerall = stateno != 132
triggerall = stateno != 140
triggerall = stateno != 150
triggerall = stateno != 151
triggerall = stateno != 152
triggerall = stateno != 153
triggerall = stateno != 154
triggerall = stateno != 155
triggerall = var(20) = 1
triggerall = Movetype = H
triggerall = p2life > 1
triggerall = life > 1
trigger1 = random <= 1
value = 501

;Sword
[State -1, AI Stab]
type = ChangeState
triggerall = power >= 3000
triggerall = var(20) = 1
triggerall = random <= 300
triggerall = p2bodydist x > 70
triggerall = StateType != A
triggerall = Movetype != H
triggerall = P2Life > 0
trigger1 = ctrl = 1
value = 845

;Sword
[State -1, AI Sharingan]
type = ChangeState
triggerall = power >= 1000
triggerall = var(20) = 1
triggerall = var(10) = 0
triggerall = StateType != A
triggerall = Movetype != H
triggerall = P2Life > 0
triggerall = life > 1
triggerall = ctrl = 1
trigger1 = random <= 10
value = 789

;Sword
[State -1, AI Fire]
type = ChangeState
triggerall = power >= 1000
triggerall = var(20) = 1
triggerall = random <= 600
triggerall = StateType != A
triggerall = Movetype != H
triggerall = P2Life > 0
triggerall = P2bodydist X >=  50
trigger1 = ctrl = 1
value = 745

;---------------------------------------------------------------------

;light punch
[State -1, AI Charge]
type = ChangeState
Triggerall = power < 2999
triggerall = var(20) = 1
triggerall = random <= 40
triggerall = StateType != A
triggerall = Movetype != H
triggerall = P2Life > 0
triggerall = P2bodydist X > 90
trigger1 = ctrl = 1
value = 930
