; The CMD file.
;
; Two parts: 1. Command definition and  2. State entry
; (state entry is after the commands def section)
;
; 1. Command definition
; ---------------------
; Note: The commands are CASE-SENSITIVE, and so are the command names.
; The eight directions are:
;   B, DB, D, DF, F, UF, U, UB     (all CAPS)
;   corresponding to back, down-back, down, downforward, etc.
; The six buttons are:
;   a, b, c, x, y, z               (all lower case)
;   In default key config, abc are are the bottom, and xyz are on the
;   top row. For 2 button characters, we recommend you use a and b.
;   For 6 button characters, use abc for kicks and xyz for punches.
;
; Each [Command] section defines a command that you can use for
; state entry, as well as in the CNS file.
; The command section should look like:
;
;   [Command]
;   name = some_name
;   command = the_command
;   time = time (optional -- defaults to 15 if omitted)
;
; - some_name
;   A name to give that command. You'll use this name to refer to
;   that command in the state entry, as well as the CNS. It is case-
;   sensitive (QCB_a is NOT the same as Qcb_a or QCB_A).
;
; - command
;   list of buttons or directions, separated by commas.
;   Directions and buttons can be preceded by special characters:
;   slash (/) - means the key must be held down
;          egs. command = /D       ;hold the down direction
;               command = /DB, a   ;hold down-back while you press a
;   tilde (~) - to detect key releases
;          egs. command = ~a       ;release the a button
;               command = ~D, F, a ;release down, press fwd, then a
;          If you want to detect "charge moves", you can specify
;          the time the key must be held down for (in game-ticks)
;          egs. command = ~30a     ;hold a for at least 30 ticks, then release
;   dollar ($) - Direction-only: detect as 4-way
;          egs. command = $D       ;will detect if D, DB or DF is held
;               command = $B       ;will detect if B, DB or UB is held
;   plus (+) - Buttons only: simultaneous press
;          egs. command = a+b      ;press a and b at the same time
;               command = x+y+z    ;press x, y and z at the same time
;   You can combine them:
;     eg. command = ~30$D, a+b     ;hold D, DB or DF for 30 ticks, release,
;                                  ;then press a and b together
;   It's recommended that for most "motion" commads, eg. quarter-circle-fwd,
;   you start off with a "release direction". This matches the way most
;   popular fighting games implement their command detection.
;
; - time (optional)
;   Time allowed to do the command, given in game-ticks. Defaults to 15
;   if omitted
;
; If you have two or more commands with the same name, all of them will
; work. You can use it to allow multiple motions for the same move.
;
; Some common commands examples are given below.
;
; [Command] ;Quarter circle forward + x
; name = "QCF_x"
; command = ~D, DF, F, x
;
; [Command] ;Half circle back + a
; name = "HCB_a"
; command = ~F, DF, D, DB, B, a
;
; [Command] ;Two quarter circles forward + y
; name = "2QCF_y"
; command = ~D, DF, F, D, DF, F, y
;
; [Command] ;Tap b rapidly
; name = "5b"
; command = b, b, b, b, b
; time = 30
;
; [Command] ;Charge back, then forward + z
; name = "charge_B_F_z"
; command = ~60$B, F, z
; time = 10
; 
; [Command] ;Charge down, then up + c
; name = "charge_D_U_c"
; command = ~60$D, U, c
; time = 10
; 

;-| Button Remapping |-----------------------------------------------------
; This section lets you remap the player's buttons (to easily change the
; button configuration). The format is:
;   old_button = new_button
; If new_button is left blank, the button cannot be pressed.
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
name = "by"
command = ~D, DF, F, b
time = 25

[Command]
name = "ju"
command = ~D, DB, B, a
time = 25
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

;===========================================================================
;---------------------------------------------------------------------------
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
[State -1, Button change]
type = ChangeState
value = ifelse(Var(39) = 0,987,9872)
triggerall = command = "s"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
;[State -1, Button change 2]
;type = ChangeState
;value = 9872
;triggerall = var(39) = 1
;triggerall = command = "s"
;trigger1 = statetype = S
;trigger1 = ctrl
;---------------------------------------------------------------------------
;Stand Light Punch
[State -1, Juuken]
type = Changestate
value = 900
triggerall = power >= 3000
triggerall = ifelse(var(39) = 0, command = "ju", command = "c")
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
;Stand Light Punch
[State -1, Byakugan]
type = Changestate
value = 850
triggerall = power >= 2000
triggerall = var(10) = 0
triggerall = ifelse(var(39) = 0, command = "by", command = "b")
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
[State -1, Wall Jump]
type = ChangeState
value = 46
triggerall = backedgebodydist <= 1 && backedgebodydist > -1
triggerall = command = "holdfwd"
triggerall = var(0) = 0
triggerall = Pos Y < -30
trigger1 = StateType = A
trigger1 = Ctrl
trigger1 = Vel X <= 0
;---------------------------------------------------------------------------
;Stand Light Punch
[State -1, Stand Throw]
type = ChangeState
value = 800
triggerall = command = "x"
triggerall = stateno = 20
triggerall = command = "holdfwd"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
; Run Forward
[State -1, Run Forward]
type = ChangeState
value = 100
trigger1 = command = "FF"
trigger1 = statetype = S
;---------------------------------------------------------------------------
; Run Back
[State -1, Run Back]
type = ChangeState
value = 105
trigger1 = command = "BB"
trigger1 = statetype = S
;---------------------------------------------------------------------------
;Stand Light Punch
[State -1, Stand Light Punch]
type = ChangeState
value = 200
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
;Stand Medium Punch
[State -1, Stand Medium Punch]
type = ChangeState
value = 210
triggerall = ifelse(var(39) = 1, command = "a", command = "b")
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 200 && animelemtime(4)>0
;---------------------------------------------------------------------------
;Stand Hard Punch
[State -1, Stand Hard Punch]
type = ChangeState
value = 220
triggerall = ifelse(var(39) = 1, command = "a", command = "c")
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 210 && animelemtime(3)>0
trigger3 = stateno = 200 && animelemtime(4)>0
trigger4 = stateno = 310 && animelemtime(3)>0
;---------------------------------------------------------------------------
;Stand Alt Hard Punch
[State -1, Stand Alt Hard Punch]
type = ChangeState
value = 230
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 200 && animelemtime(3)>0
trigger3 = stateno = 210 && animelemtime(3)>0
trigger4 = stateno = 220 && animelemtime(3)>0
trigger5 = stateno = 310 && animelemtime(3)>0
;---------------------------------------------------------------------------
;Stand Teleport
[State -1, Stand To Stand Teleport]
type = ChangeState
value = 300
triggerall = command = "holddown"
triggerall = command = "a"
trigger1 = statetype = C
trigger1 = ctrl
;---------------------------------------------------------------------------
;Stand Teleport
[State -1, Air to Stand Teleport]
type = ChangeState
value = 301
triggerall = command != "holddown"
triggerall = command = "a"
trigger1 = statetype = A
trigger1 = ctrl
;---------------------------------------------------------------------------
;Stand Teleport
[State -1, Stand to Air Teleport]
type = ChangeState
value = 302
triggerall = command = "holddown"
triggerall = command = "b"
trigger1 = statetype = C
trigger1 = ctrl
;---------------------------------------------------------------------------
;Stand Light Punch
[State -1, Stand Kunai throwh]
type = ChangeState
value = 310
triggerall = var(40) < 8
triggerall = command = "y"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 200 && animelemtime(3)>0
trigger3 = stateno = 210 && animelemtime(3)>0
;----------------------------------------------------------------------------
[State -1, Kavamari]
type = ChangeState
value = 500
triggerall = p2stateno = 900
triggerall = p2stateno = 901
triggerall = p2stateno = 902
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
;---------------------------------------------------------------------------
;Stand Light Punch
[State -1, Air Light Punch'es]
type = ChangeState
value = 600
triggerall = prevstateno != 600
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 610 && animelemtime(3)>0
;---------------------------------------------------------------------------
;Stand Light Punch
[State -1, Air Heavy Punch]
type = ChangeState
value = 610
triggerall = prevstateno != 610
triggerall = command = "y"
triggerall = command != "holddown"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 600 && animelemtime(3)>0
