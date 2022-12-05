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
;   time = time (optional)
;   buffer.time = time (optional)
;
; - some_name
;   A name to give that command. You'll use this name to refer to
;   that command in the state entry, as well as the CNS. It is case-
;   sensitive (QCB_a is NOT the same as Qcb_a or QCB_A).
;
; - command
;   list of buttons or directions, separated by commas. Each of these
;   buttons or directions is referred to as a "symbol".
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
;   greater-than (>) - means there must be no other keys pressed or released
;                      between the previous and the current symbol.
;          egs. command = a, >~a   ;press a and release it without having hit
;                                  ;or released any other keys in between
;   You can combine the symbols:
;     eg. command = ~30$D, a+b     ;hold D, DB or DF for 30 ticks, release,
;                                  ;then press a and b together
;
;   Note: Successive direction symbols are always expanded in a manner similar
;         to this example:
;           command = F, F
;         is expanded when MUGEN reads it, to become equivalent to:
;           command = F, >~F, >F
;
;   It is recommended that for most "motion" commads, eg. quarter-circle-fwd,
;   you start off with a "release direction". This makes the command easier
;   to do.
;
; - time (optional)
;   Time allowed to do the command, given in game-ticks. The default
;   value for this is set in the [Defaults] section below. A typical
;   value is 15.
;
; - buffer.time (optional)
;   Time that the command will be buffered for. If the command is done
;   successfully, then it will be valid for this time. The simplest
;   case is to set this to 1. That means that the command is valid
;   only in the same tick it is performed. With a higher value, such
;   as 3 or 4, you can get a "looser" feel to the command. The result
;   is that combos can become easier to do because you can perform
;   the command early. Attacks just as you regain control (eg. from
;   getting up) also become easier to do. The side effect of this is
;   that the command is continuously asserted, so it will seem as if
;   you had performed the move rapidly in succession during the valid
;   time. To understand this, try setting buffer.time to 30 and hit
;   a fast attack, such as KFM's light punch.
;   The default value for this is set in the [Defaults] section below. 
;   This parameter does not affect hold-only commands (eg. /F). It
;   will be assumed to be 1 for those commands.
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
command.time = 15

; Default value for the "buffer.time" parameter of a Command. Minimum 1,
; maximum 30.
command.buffer.time = 1


;-| Super Motions |--------------------------------------------------------
;The following two have the same name, but different motion.
;Either one will be detected by a "command = TripleKFPalm" trigger.
;Time is set to 20 (instead of default of 15) to make the move
;easier to do.
;

;-| Special Motions |------------------------------------------------------
[Command]
name = "rasengan"
command = D, F, b
time = 30

[Command]
name = "explode"
command = D, F, y
time = 30

[Command]
name = "kuchiyose"
command = D, B, z
time = 30

[Command]
name = "flash"
command = D, F, x
time = 30

[Command]
name = "tajuu"
command = D, F, z
time = 30

[Command]
name = "firerasengan"
command = D, B, D, B, b
time = 40

[Command]
name = "dotonrasengan"
command = D, F, D, F, a+b
time = 40

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
;Run Fwd
[State -1, Run Fwd]
type = ChangeState
value = 100
trigger1 = command = "FF"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
;Run Back
[State -1, Run Back]
type = ChangeState
value = 105
trigger1 = command = "BB"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
;Weaker Rasengan
[State -1, Weak]
type = changestate
value = 1877
triggerall = life <= 300
triggerall = power >= 3000
triggerall = command = "dotonrasengan"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
;Really Weak Rasengan
[State -1, secret]
type = changestate
value = 1440
triggerall = command = "firerasengan"
triggerall = life <= 500
triggerall = power >= 3000
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
;Combo
[State -1, Stand Strong Punch1]
type = ChangeState
value = 200
triggerall = command = "a"
triggerall = command != "holddown"
triggerall = StateNo != 100
trigger1 = statetype = S
trigger1 = ctrl

[State -1, Stand Strong Punch2]
type = ChangeState
value = 210
triggerall = stateno = 200
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 8

[State -1, Stand Strong Punch3]
type = ChangeState
value = 220
triggerall = stateno = 210
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 12

;---------------------------------------------------------------------------
;Flash Move
[State -1, Flash Move]
type = ChangeState
value = 1221
triggerall = command = "flash"
triggerall = life <= 750
triggerall = power >= 3000
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
[State -1, WRYondaime Kawarimi]
type = ChangeState
value = 905
triggerall = command = "c"
triggerall = power >= 100
trigger1 = movetype = H && (statetype = S)

[State -1, WRYondaime Dash Punch]
type = ChangeState
value = 285
triggerall = stateno = 905
triggerall = command = "b"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 5

;---------------------------------------------------------------------------
;Spin Attack
[State -1, Spin Attack]
type = ChangeState
value = 225
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
triggerall = StateNo = 100
trigger1 = ctrl

;---------------------------------------------------------------------------
[State -1, WRYondaime Strong Kick]
type = ChangeState
value = 222
triggerall = command = "x"
triggerall = StateNo = 100
triggerall = statetype = S
trigger1= ctrl

[State -1, WRYondaime Dash Kick]
type = ChangeState
value = 223
triggerall = stateno = 222
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 5

;---------------------------------------------------
;Explosive Kunai
[State -1, ExplodeKunai]
type = changestate
value = 1331
triggerall = power >= 1500
triggerall = command = "explode"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------
;Kunai Throw
[State -1, kunaithrow]
type = ChangeState
value = 1234
triggerall = command = "y"
trigger1 = statetype =S
trigger1 = ctrl

;Kunai Throw(double)
[State -1, kunaidouble]
type = ChangeState
value = 123456
triggerall = command = "y"
triggerall = stateno = 1234
trigger1 = time >= 12
trigger1 = ctrl
trigger2 = statetype = S

;----------------------------------------------------------
;Rasengan
[State -1, rasengan]
type = ChangeState
value = 777
triggerall = command = "rasengan"
triggerall = power >= 3000
trigger1 = statetype = S
trigger1 = ctrl = 1

;------------------------------------------------------------
;Taren Ken
[State -1, taren ken]
type = ChangeState
value = 999
triggerall = command = "c"
triggerall = power >= 1000
trigger1 = statetype = C
trigger1 = ctrl = 1

;------------------------------------------------------------
;Tajuu Kage bunshin no Jutsu
[State -1, Tajuu]
type = Changestate
value = 8060
triggerall = var(40) = 0
triggerall = power >= 3000
triggerall = command = "tajuu"
trigger1 = numhelper(8000) < 1
trigger1 = statetype != A
trigger1 = statetype = S
trigger1 = ctrl

;------------------------------------------------------------
;Katon
[State -1, Katon]
type = ChangeState
value = 333
triggerall = command = "z"
triggerall = power >= 1500
trigger1 = statetype = S
trigger1 = ctrl = 1

;----------------------------------------------------------------
;rasengan2
[state -1, rasengan2]
type = changestate
value = 778
triggerall = command = "b"
triggerall = power >= 2000
trigger1 = statetype = C
trigger1 = ctrl = 1

;----------------------------------------------------------------
;Weak Rasnegan
[State -1, weak rasengan]
type = changestate
value = 888
triggerall = command = "b"
triggerall = power >=1000
trigger1 = statetype = S
trigger1 = ctrl = 1

;----------------------------------------------------------------
[State -1, Stand Strong Punch1]
type = ChangeState
value = 213
triggerall = command = "x"
triggerall = command != "holddown"
triggerall = StateNo != 100
trigger1 = statetype = S
trigger1 = ctrl

[State -1, WRYondaime Strong Kick]
type = ChangeState
value = 243
trigger1 = stateno = 213
trigger1 = movecontact
triggerall = command = "x"
trigger2 = statetype = S
trigger2 = ctrl

[State -1, WRYondaime Dash Kick]
type = ChangeState
value = 244
triggerall = stateno = 243
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 5

[State -1, kunaidouble]
type = ChangeState
value = 123456
triggerall = command = "x"
trigger1 = stateno = 244
trigger1 = movecontact
trigger2 = ctrl
trigger2 = statetype = S

;----------------------------------------------------
;Kage bunshin no Jutsu
[State -1, Kage bunshin no Jutsu]
type = ChangeState
value = 8000
triggerall = var(40) = 0
triggerall = power >= 1000
triggerall = command = "z"
trigger1 = numhelper(8000) < 1
trigger1 = statetype = C
trigger1 = ctrl

;------------------------------------------------------
;Jump Attack
[State -1, jumpattack]
type = changestate
value = 555
triggerall = command = "a"
trigger1 = statetype != S
trigger1 = statetype = A
trigger1 = ctrl = 1

;---------------------------------------
;Jump Attack 2
[State -1, jumpattack2]
type = changestate
value = 555
triggerall = command = "x"
trigger1 = statetype !=S
trigger1 = statetype = A
trigger1 = ctrl = 1

;-----------------------------------------
;Duck Kick
[State -1, duckkick]
type = changestate
value = 556
triggerall = command = "a"
trigger1 = statetype = C
trigger1 = ctrl = 1

;--------------------------------------------
;Stun Kunai Throw
[State -1, kunaithrow]
type = ChangeState
value = 54321
triggerall = command = "y"
trigger1 = statetype = C
trigger1 = ctrl

;--------------------------------------------------
;Teleport
[State -1, Teleport]
type = changestate
value = 989
triggerall = command = "start"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------
;Duck Kick
[State -1, duckkick]
type = changestate
value = 556
triggerall = command = "x"
trigger1 = statetype = C
trigger1 = ctrl = 1
