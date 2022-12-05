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

;-| Super Motions |--------------------------------------------------------
;The following two have the same name, but different motion.
;Either one will be detected by a "command = TripleKFPalm" trigger.
;Time is set to 20 (instead of default of 15) to make the move
;easier to do.
;
[Command]
name = "TripleKFPalm"
command = ~D, DF, F, D, DF, F, x
time = 20

[Command] 
name = "TripleKFPalm"   ;Same name as above
command = ~D, DF, F, D, DF, F, y
time = 20

;-| Stun/Chakra |--------------------------------------------------------
[Command]
name = "Stun"
command = /b

[Command]
name = "chakra"
command = /z

;-| Special Motions |------------------------------------------------------

[Command]
name = "ShikaUltra"
command = ~D, DF, F, z

[Command]
name = "QCF_x"
command = ~D, DF, F, x

[Command]
name = "QCF_y"
command = ~D, DF, F, y

[Command]
name = "QCF_xy"
command = ~D, DF, F, x+y

[Command]
name = "QCB_a"
command = ~D, DF, F, a

[Command]
name = "QCB_b"
command = ~D, DF, F, b

[Command]
name = "FF_ab"
command = F, F, a+b

[Command]
name = "FF_a"
command = F, F, a

[Command]
name = "FF_b"
command = F, F, b

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
name = "recovery";Required (do not remove)
command = x+y
time = 1

;-| Dir + Button |---------------------------------------------------------
[Command]
name = "down_a"
command = /$D,a
time = 1

[Command]
name = "down_b"
command = /$D,b
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
name = "yy"
command = y, y
time = 1

[Command]
name = "z"
command = z
time = 1

[Command]
name = "start"
command = s
time = 1

;-| Hold Dir |--------------------------------------------------------------
[Command]
name = "holdfwd";Required (do not remove)
command = /$F
time = 1

[Command]
name = "holdback";Required (do not remove)
command = /$B
time = 1

[Command]
name = "holdup" ;Required (do not remove)
command = /$U
time = 1

[Command]
name = "holddown";Required (do not remove)
command = /$D
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
;C- Shikamaru Run Foward
[State -1, C-Shikamaru Run Foward]
type = ChangeState
value = 100
trigger1 = command = "FF"
trigger1 = statetype = S
trigger1 = ctrl

[State -1, C Shikamaru Dash Punch]
type = ChangeState
value = 210
triggerall = stateno = 100
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 1

[State -1, C Shikamaru Dash Stab]
type = ChangeState
value = 444
triggerall = stateno = 100
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 1
;---------------------------------------------------------------------------
;C-Shikamaru Run Back
[State -1, C-Shikamaru Run Back]
type = ChangeState
value = 10005
trigger1 = command = "BB"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
;Taunt
[State -1, Taunt]
type = ChangeState
value = 195
triggerall = command = "start"
triggerall = statetype != A
trigger1 = statetype != A
trigger1 = ctrl
;---------------------------------------------------------------------------
;C Shikamaru Punch Combo
[State -1, C Shikamaru Punch Combo]
type = ChangeState
value = 200
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 210
trigger2 = time >= 30

[State -1, C Shikamaru Punch Combo 2]
type = ChangeState
value = 205
triggerall = stateno = 200
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 7

[State -1, C Shikamaru Punch Combo 3]
type = ChangeState
value = 210
triggerall = stateno = 205
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 7
;---------------------------------------------------------------------------
;C Shikamaru Kick Combo
[State -1, C Shikamaru Kick Combo]
type = ChangeState
value = 453
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 455
trigger2 = time >= 30

[State -1, C Shikamaru Kick Combo 2]
type = ChangeState
value = 454
triggerall = stateno = 453
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 7

[State -1, C Shikamaru Kick Combo 3]
type = ChangeState
value = 455
triggerall = stateno = 454
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 9

[State -1, C Shikamaru Kunai Combo]
type = ChangeState
value = 444
triggerall = stateno = 454
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 7
;---------------------------------------------------------------------------
;C-Shikamaru Kunai Throw
[State -1, C-Shikamaru Kunai Throw]
type = ChangeState
value = 225
triggerall = command = "y"
trigger1 = statetype = S
triggerall = ctrl
trigger2 = stateno = 228

;C-Shikamaru Double Kunai Throw
[State -1, C-Shikamaru Double Kunai Throw]
type = ChangeState
value = 228
trigger1 = statetype = S
triggerall = command = "y"
trigger1 = ctrl
triggerall = time >= 18
trigger2 = stateno = 225
;--------------------------------------------------------------------------
;C-Shikamaru Special Stun
[State -1, C-Shikamaru Special Stun]
type = ChangeState
value = 160
triggerall = command = "Stun"
triggerall = command != "holddown"
triggerall = power >= 35
trigger1 = statetype = S
trigger1 = ctrl
trigger1 = time >= 10

;C-Shikamaru Summon Asuma
[State -1, C-Shikamaru Summon Asuma]
type = ChangeState
value = 410
triggerall = command = "y"
triggerall = command = "holddown"
triggerall = power >= 1000
triggerall = stateno = 160
trigger1 = ctrl
trigger1 = time >= -20
trigger2 = movecontact

;C-Shikamaru Summon Dad
[State -1, C-Shikamaru Summon Dad]
type = ChangeState
value = 414
triggerall = command = "z"
triggerall = command = "holddown"
triggerall = power >= 2000
triggerall = stateno = 160
trigger1 = ctrl
trigger1 = time >= -20
trigger2 = movecontact

;C-Shikamaru Summon Temari
[State -1, C-Shikamaru Summon Temari]
type = ChangeState
value = 430
triggerall = command = "x"
triggerall = command = "holddown"
triggerall = power >= 1000
triggerall = stateno = 160
trigger1 = ctrl
trigger1 = time >= -20
trigger2 = movecontact

;C-Shikamaru Summon Temari 2
[State -1, C-Shikamaru Summon Temari]
type = ChangeState
value = 431
triggerall = command = "x"
triggerall = command = "holdup"
triggerall = power >= 2000
triggerall = stateno = 160
trigger1 = ctrl
trigger1 = time >= -20
trigger2 = movecontact

;---------------------------------------------------------------------------


;---------------------------------------------------------------------------
;C-Shikamaru Shadow Grab
[State -1, C-Shikamaru Shadow Grab]
type = ChangeState
value = 800
triggerall = command = "a"
triggerall = command = "holddown"
triggerall = power >= 1500
triggerall = stateno = 160
trigger1 = ctrl
trigger1 = time >= 10
trigger2 = movecontact
;---------------------------------------------------------------------------
;C-Shikamaru Duck Punch
[State -1, C-Shikamaru Duck Punch]
type = ChangeState
value = 406
triggerall = command = "x"
triggerall = command = "holddown"
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = movecontact
trigger2 = time = 8
;---------------------------------------------------------------------------
;C-Shikamaru Jump Kick
[State -1, C-Shikamaru Jump Kick]
type = ChangeState
value = 395
triggerall = command = "a"
trigger1 = statetype = A
trigger1 = ctrl
;---------------------------------------------------------------------------
;C-Shikamaru Jump Kick x2
[State -1, C-Shikamaru Jump Kick x2]
type = ChangeState
value = 395
triggerall = command = "x"
trigger1 = statetype = A
trigger1 = ctrl
;---------------------------------------------------------------------------
;C-Shikamaru Duck Kick
[State -1, C-Shikamaru Duck Kick]
type = ChangeState
value = 406
triggerall = command = "a"
triggerall = command = "holddown"
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = movecontact
trigger2 = time = 8
;---------------------------------------------------------------------------
;C-Shikamaru Kawarimi
[State -1, C-Shikamaru Kawarimi]
type = ChangeState
value = 905
triggerall = command = "c"
triggerall = power >= 100
trigger1 = movetype = H && (statetype = S)

[State -1, C Shikamaru Dash Punch]
type = ChangeState
value = 285
triggerall = stateno = 905
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 5

;---------------------------------------------------------------------------
;C-Shikamaru Shadow Get-Hit
[State -1, C-Shikamaru hadow Get-Hit]
type = ChangeState
value = 323
triggerall = stateno = 160
trigger1 = movetype = H
;----------------------------------------------------------------------------
;C-Shikamaru Chakra Gain
[State -1, C-Shikamaru Chakra Gain]
type = ChangeState
value = 700
triggerall = command = "chakra"
triggerall = stateno = 11
triggerall = power < 2999
trigger2 = statetype != A
trigger1 = ctrl
;--------------------------------------------------------------------------
;C-Shikamaru Ultra
[State -1, C-Shikamaru Ultra]
type = ChangeState
value = 1005
triggerall = command = "ShikaUltra"
triggerall = power >= 3000
trigger1 = statetype != A
trigger1 = ctrl

;C-Shikamaru Throw Ball/Kunai
[State -1, C-Shikamaru Throw Ball/Kunai]
type = changestate
value = 2000
triggerall = stateno = 106
trigger1 = statetype = S
trigger1 = time >= 1

;C-Shikamaru Throw Burning Kunai
[State -1, C-Shikamaru Throw Burning Kunai]
type = changestate
value = 556
triggerall = stateno = 2000
trigger1 = statetype = S
trigger1 = time >= 24

;C-Shikamaru Kagename Sucess !
[State -1, C-Shikamaru Kagename Sucess !]
type = changestate
value = 1601
triggerall = stateno = 556
trigger1 = statetype = S
trigger1 = time >= 24


--------------------------------------------------

