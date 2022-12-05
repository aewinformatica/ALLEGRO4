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
;; 

;-| Super Motions |--------------------------------------------------------
;The following two have the same name, but different motion.
;Either one will be detected by a "command = TripleKFPalm" trigger.
;Time is set to 20 (instead of default of 15) to make the move
;easier to do.
;

;-| Special Motions |------------------------------------------------------

[Command]
name = "FDFx"
command = ~F,D,DF,x
time = 15

[Command]
name = "FDFa"
command = ~F,D,DF,a
time = 15

[Command]
name = "DFDFy"
command = ~D,F,D,F,y
time = 24

[Command]
name = "DFDFb"
command = ~D,F,D,F,b
time = 24

[Command]
name = "DFDFx"
command = ~D,F,D,F,x
time = 24

[Command]
name = "DFDFa"
command = ~D,F,D,F,a
time = 24

[Command]
name = "BDFy"
command = ~D,DF,F,y
time = 15

[Command]
name = "BDFb"
command = ~D,DF,F,b
time = 15

[Command]
name = "DFx"
command = ~D,DF,F,x
time = 15

[Command]
name = "DFa"
command = ~D,DF,F,a
time = 15

[Command]
name = "DBx"
command = ~D,DB,B,x
time = 15

[Command]
name = "DBa"
command = ~D,DB,B,a
time = 15

[Command]
name = "DBy"
command = ~D,DB,B,y
time = 15

[Command]
name = "DBb"
command = ~D,DB,B,b
time = 15

[Command]
name = "DFy"
command = ~D,DF,F,y
time = 15

[Command]
name = "DFb"
command = ~D,DF,F,b
time = 15

[Command]
name = "fwd_x"
command = /F,x
time = 1

[Command]
name = "Dy"
command = /D,y
time = 1

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
name = "fwd_a"
command = /F,a
time = 1

[Command]
name = "fwd_b"
command = /F,b
time = 1

[Command]
name = "back_a"
command = /B,a
time = 1

[Command]
name = "back_b"
command = /B,b
time = 1

[Command]
name = "recovery";Required (do not remove)
command = x+y
time = 1

[Command]
name = "xy"
command = x+y
time = 1

[Command]
name = "ab"
command = a+b
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
name = "z"
command = z
time = 1

[Command]
name = "s"
command = s
time = 1

[Command]
name = "back"
command = B
time = 1

[Command]
name = "upback"
command = UB
time = 1

[Command]
name = "downback"
command = DB
time = 1

[Command]
name = "downfwd"
command = /$DF
time = 1

[Command]
name = "y1"
command = /y
time = 5

[Command]
name = "a1"
command = /a
time = 5

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

;longjump
[Command]
name = "longjump"
command = D, $U
time = 18

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
;---------------------------------------------------------------------------
;DFDFaC
[State -1]
type = ChangeState
value = 61599
triggerall = command = "DFDFa"
triggerall = power >= 3000
triggerall = statetype != A
triggerall = movecontact
trigger1 = stateno = [740,770]
trigger2 = stateno = 677
trigger3 = stateno = [6771,6772]


;---------------------------------------------------------------------------
;DFDFxC
[State -1]
type = ChangeState
value = 61699
triggerall = command = "DFDFx"
triggerall = power >= 2000
triggerall = P2bodydist X <= 100
triggerall = statetype != A
triggerall = movecontact
trigger1 = stateno = [740,770]
trigger2 = stateno = 677
trigger3 = stateno = [6771,6772]

;---------------------------------------------------------------------------
;DFDFa
[State -1]
type = ChangeState
value = 615
triggerall = command = "DFDFa"
triggerall = power >= 2000
triggerall = statetype != A
trigger1 = ctrl = 1
trigger2 = stateno = [199,208]
trigger2 = movecontact
trigger3 = stateno = 2071
trigger3 = movecontact
trigger4 = stateno = 307
trigger4 = movecontact


;---------------------------------------------------------------------------
;DFDFx
[State -1]
type = ChangeState
value = 616
triggerall = command = "DFDFx"
triggerall = power >= 1000
triggerall = statetype != A
trigger1 = ctrl = 1
trigger2 = stateno = [199,208]
trigger2 = movecontact
trigger3 = stateno = 2071
trigger3 = movecontact
trigger4 = stateno = 307
trigger4 = movecontact

;---------------------------------------------------------------------------
;DBx
[State -1]
type = ChangeState
value = 740
triggerall = command = "DBa"
triggerall = statetype != A
trigger1 = ctrl = 1
trigger2 = stateno = [199,208]
trigger2 = movecontact
trigger3 = stateno = 2071
trigger3 = movecontact
trigger4 = stateno = 307
trigger4 = movecontact


;---------------------------------------------------------------------------
;DBx
[State -1]
type = ChangeState
value = 740
triggerall = command = "DBx"
triggerall = statetype != A
trigger1 = ctrl = 1
trigger2 = stateno = [199,208]
trigger2 = movecontact
trigger3 = stateno = 2071
trigger3 = movecontact
trigger4 = stateno = 307
trigger4 = movecontact

;---------------------------------------------------------------------------
;DBy
[State -1]
type = ChangeState
value = 750
triggerall = command = "DBy"
triggerall = statetype != A
trigger1 = ctrl = 1
trigger2 = stateno = [199,208]
trigger2 = movecontact
trigger3 = stateno = 2071
trigger3 = movecontact
trigger4 = stateno = 307
trigger4 = movecontact

;---------------------------------------------------------------------------
;DBb
[State -1]
type = ChangeState
value = 750
triggerall = command = "DBb"
triggerall = statetype != A
trigger1 = ctrl = 1
trigger2 = stateno = [199,208]
trigger2 = movecontact
trigger3 = stateno = 2071
trigger3 = movecontact
trigger4 = stateno = 307
trigger4 = movecontact

;------------------------------
;DFb
[State -1]
type = ChangeState
value = 770
triggerall = command = "DFb"
triggerall = statetype != A
trigger1 = ctrl = 1
trigger2 = stateno = [199,208]
trigger2 = movecontact
trigger3 = stateno = 2071
trigger3 = movecontact
trigger4 = stateno = 307
trigger4 = movecontact

;------------------------------
;DFy
[State -1]
type = ChangeState
value = 760
triggerall = command = "DFy"
triggerall = statetype != A
trigger1 = ctrl = 1
trigger2 = stateno = [199,208]
trigger2 = movecontact
trigger3 = stateno = 2071
trigger3 = movecontact
trigger4 = stateno = 307
trigger4 = movecontact



;---------------------------------------------------------------------------
;DFx
[State -1]
type = ChangeState
value = 677
triggerall = command = "DFx"
triggerall = statetype != A
trigger1 = ctrl = 1
trigger2 = stateno = [199,208]
trigger2 = movecontact
trigger3 = stateno = 2071
trigger3 = movecontact
trigger4 = stateno = 307
trigger4 = movecontact

;---------------------------------------------------------------------------
;DFa
[State -1]
type = ChangeState
value = 677
triggerall = command = "DFa"
triggerall = statetype != A
trigger1 = ctrl = 1
trigger2 = stateno = [199,208]
trigger2 = movecontact
trigger3 = stateno = 2071
trigger3 = movecontact
trigger4 = stateno = 307
trigger4 = movecontact


;Bxy
[State -1]
type = ChangeState
value = 655
trigger1 = command = "x"
trigger1 = command = "y"
trigger1 = command = "holdback"
trigger1 = statetype = S
trigger1 = ctrl = 1

;Bxy
[State -1]
type = ChangeState
value = 655
trigger1 = command = "c"
trigger1 = command = "holdback"
trigger1 = statetype = S
trigger1 = ctrl = 1

;GBxy
[State -1]
type = ChangeState
value = 6551
triggerall = command = "x"
triggerall = command = "y"
triggerall = command = "holdback"
triggerall = power >= 1000
triggerall = statetype != A
trigger1 = stateno = 150
trigger2 = stateno = 152

;GBxy
[State -1]
type = ChangeState
value = 6551
triggerall = command = "c"
triggerall = command = "holdback"
triggerall = power >= 1000
triggerall = statetype != A
trigger1 = stateno = 150
trigger2 = stateno = 152

;Fxy
[State -1]
type = ChangeState
value = 652
trigger1 = command = "x"
trigger1 = command = "y"
trigger1 = statetype = S
trigger1 = ctrl = 1

;Fxy
[State -1]
type = ChangeState
value = 652
trigger1 = command = "c"
trigger1 = statetype = S
trigger1 = ctrl = 1

;GFxy
[State -1]
type = ChangeState
value = 6521
triggerall = command = "x"
triggerall = command = "y"
triggerall = power >= 1000
triggerall = statetype != A
trigger1 = stateno = 150
trigger2 = stateno = 152

;GFxy
[State -1]
type = ChangeState
value = 6521
triggerall = command = "c"
triggerall = power >= 1000
triggerall = statetype != A
trigger1 = stateno = 150
trigger2 = stateno = 152

;SPG_BD
[State -1]
type = ChangeState
value = 50799
triggerall = statetype != A
triggerall = alive = 1
triggerall = life != 0
triggerall = movetype != A
triggerall = stateno != 5277
triggerall = stateno != 5299
trigger1 = anim >= 5090
trigger1 = anim <= 5105
trigger1 = command = "xy"
trigger2 = anim >= 5090
trigger2 = anim <= 5105
trigger2 = command = "c"
trigger3 = anim >= 5090
trigger3 = anim <= 5105
trigger3 = command = "ab"
trigger4 = anim = [5110,5150]
trigger4 = time <=5
trigger4 = command = "c"
trigger5 = anim = [5110,5150]
trigger5 = time <=5
trigger5 = command = "xy"
trigger6 = anim = [5110,5150]
trigger6 = time <=5
trigger6 = command = "ab"


;---------------------------------------------------------------------------
;Run Fwd
;ダッシ?
[State -1, Run Fwd]
type = ChangeState
value = 100
trigger1 = command = "FF"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
;Run Back
;後退ダッシ?
[State -1, Run Back]
type = ChangeState
value = 105
trigger1 = command = "BB"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
;NFA
[State -1]
type = ChangeState
value = 679
triggerall = statetype = S
triggerall = stateno != 100
triggerall = p2movetype != H
triggerall = (p2statetype = S) || (p2statetype = C)
triggerall = P2bodydist X <= 6
triggerall = ctrl = 1
trigger1 = command = "fwd_a"
trigger2 = command = "back_a"
trigger3 = command = "fwd_b"
trigger4 = command = "back_b"


;---------------------------------------------------------------------------
;stcd
[State -1]
type = ChangeState
value = 2090
trigger1 = command = "ab"
triggerall = statetype != A
trigger1 = ctrl = 1

[State -1]
type = ChangeState
value = 2090
trigger1 = command = "z"
triggerall = statetype != A
trigger1 = ctrl = 1


;stcdG
[State -1]
type = ChangeState
value = 2091
triggerall = command = "ab"
triggerall = power >= 1000
triggerall = statetype != A
trigger1 = stateno = 150
trigger2 = stateno = 152

[State -1]
type = ChangeState
value = 2091
triggerall = command = "z"
triggerall = power >= 1000
triggerall = statetype != A
trigger1 = stateno = 150
trigger2 = stateno = 152

;---------------------------------------------------------------------------
;Fx
[State -1, Stand Light Kick]
type = ChangeState
value = 307
triggerall = command = "fwd_x"
triggerall = statetype != A
trigger1 = stateno = [199,208]
trigger1 = movecontact
trigger2 = ctrl
trigger3 = stateno = 2071
trigger3 = movecontact

;---------------------------------------------------------------------------
[State -1, Stand Light Punch]
type = ChangeState
value = 199
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = (stateno = 200) && time > 6
trigger2 = stateno = 200
trigger2 = movecontact

;---------------------------------------------------------------------------
[State -1, Stand Strong Punch]
type = ChangeState
value = 202
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = (stateno = 199) && time > 6
trigger2 = stateno = 199
trigger2 = movecontact

;---------------------------------------------------------------------------
;Stand Light Punch
[State -1, Stand Light Punch]
type = ChangeState
value = 200
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
;STAN
[State -1, Stand Strong Punch]
type = ChangeState
value = 202
triggerall = command = "a"
triggerall = command != "holddown"
triggerall = p2bodydist x <= 15
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 205
trigger2 = movecontact

;---------------------------------------------------------------------------
;STA
[State -1, Stand Strong Punch]
type = ChangeState
value = 203
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = (stateno = 202) && time > 7
trigger3 = stateno = 202
trigger3 = movecontact
trigger4 = stateno = 205
trigger4 = movecontact

;---------------------------------------------------------------------------
;STYN
[State -1, Stand Light Kick]
type = ChangeState
value = 205
triggerall = command = "y"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 200
trigger2 = movecontact

;---------------------------------------------------------------------------
;STBN
[State -1, 1]
type = ChangeState
value = 207
triggerall = command = "b"
triggerall = command != "holddown"
triggerall = p2bodydist x <= 23
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 200
trigger2 = movecontact
trigger3 = stateno = 205
trigger3 = movecontact
;---------------------------------------------------------------------------
;STB
[State -1, 1]
type = ChangeState
value = 2071
triggerall = command = "b"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 200
trigger2 = movecontact
trigger3 = stateno = 201
trigger3 = movecontact

;---------------------------------------------------------------------------
;Taunt
;挑発
;[State -1, Taunt]
;type = ChangeState
;value = 195
;triggerall = command = "s"
;trigger1 = statetype != A
;trigger1 = ctrl

;---------------------------------------------------------------------------
;Cx
[State -1, Crouching Light Punch]
type = ChangeState
value = 201
triggerall = command = "x"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = (stateno = 201) && time > 9
trigger3 = (stateno = 208) && time > 9

;---------------------------------------------------------------------------
;Cy
[State -1, Crouching Light Punch]
type = ChangeState
value = 208
triggerall = command = "y"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = (stateno = 201) && time > 9
trigger3 = (stateno = 208) && time > 9
trigger4 = stateno = 201
trigger4 = movecontact

;---------------------------------------------------------------------------
;Ca
[State -1, Crouching Strong Punch]
type = ChangeState
value = 206
triggerall = command = "a"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = stateno = 208
trigger2 = movecontact
trigger3 = stateno = 201
trigger3 = movecontact

;---------------------------------------------------------------------------
;Cb
[State -1, Crouching Strong Kick]
type = ChangeState
value = 210
triggerall = command = "b"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = stateno = 208
trigger2 = movecontact
trigger3 = stateno = 201
trigger3 = movecontact
;---------------------------------------------------------------------------
;jcd
[State -1, Crouching Light Punch]
type = ChangeState
value = 6221
triggerall = statetype = A
triggerall = ctrl
trigger1 = command = "ab"
trigger2 = command = "z"

;---------------------------------------------------------------------------
;jx
[State -1, 1]
type = ChangeState
value = 620
triggerall = command = "x"
trigger1 = statetype = A
trigger1 = ctrl

;---------------------------------------------------------------------------
;jy
[State -1, 1]
type = ChangeState
value = 605
triggerall = command = "y"
trigger1 = statetype = A
trigger1 = ctrl

;---------------------------------------------------------------------------
;Ja
[State -1, Jump Strong Punch]
type = ChangeState
value = 606
triggerall = command = "a"
trigger1 = statetype = A
trigger1 = ctrl

;---------------------------------------------------------------------------
;Jb
[State -1, Jump Strong Punch]
type = ChangeState
value = 621
triggerall = command = "b"
trigger1 = statetype = A
trigger1 = ctrl

