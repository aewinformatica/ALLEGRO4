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

[Command]
name = "down_y"
command = /$D,y
time = 1

[Command]
name = "down_c"
command = /$D,c
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



;-| AI |--------------------------------------------------------------


[Command]
name = "CPU1"
command = D, D, U, U, D, U
time = 1

[Command]
name = "CPU2"
command = D, U, D, a+b, D
time = 1


[Command]
name = "CPU3"
command = D, U, U, a+b, D
time = 1


[Command]
name = "CPU4"
command = D, U, D, a+b, U
time = 1

[Command]
name = "CPU5"
command = D, U, D, c+b, D
time = 1

[Command]
name = "CPU6"
command = D, U, D, z+b, D
time = 1


[Command]
name = "CPU7"
command = D, U, D, y+b, D
time = 1

[Command]
name = "CPU8"
command = D, F, D, z+b, D
time = 1


[Command]
name = "CPU9"
command = B, U, D, z+b, D
time = 1

[Command]
name = "CPU10"
command = B, F, D, z+b, D
time = 1


[Command]
name = "CPU11"
command = B, F, D, z+b, D
time = 1

[Command]
name = "CPU12"
command = B, F, D, z+b, U
time = 1

[Command]
name = "CPU13"
command = B, F, D, a+z, D
time = 1

[Command]
name = "CPU14"
command = B, F, U, y+b, D
time = 1

[Command]
name = "CPU15"
command = D, F, U, z+c, D
time = 1




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

;---------------------------------------------------------------------------
;c1 a1
[State -1]
Type = ChangeState
Triggerall = Command = "a"
Triggerall = Command != "holddown"
Trigger1 = ctrl = 1
Trigger1 = StateType = S
Value = 200
trigger2 = stateno = 250
trigger2 = time >= 16

;c1 a2
[State -1]
type = ChangeState
value = 210
triggerall = stateno = 200
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 9


;c1 a3
[State -1]
type = ChangeState
value = 230
triggerall = stateno = 210
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 9


;---------------------------------------------------------------------------
;c2 a1
[State -1]
Type = ChangeState
Triggerall = Command = "x"
Triggerall = Command != "holddown"
Trigger1 = ctrl = 1
Trigger1 = StateType = S
trigger2 = stateno = 210
trigger2 = time >= 9
Value = 240

;c2 a2
[State -1]
type = ChangeState
value = 250
triggerall = stateno = 240
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 10

;c2 a3
[State -1]
type = ChangeState
value = 260
triggerall = stateno = 250
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 16

;----------------------------------------------------------
;Kawarimi
[State -1, Kawarimi]
type = ChangeState
value = 505
triggerall = command = "c"
trigger1 = movetype = H && (statetype = S)


;Kawarimi distanced
[State -1, Kawarimi2]
type = ChangeState
value = 507
triggerall = command = "c"
trigger1 = movetype = H && (statetype = S)












;AA
[State -1,AA]
Type = ChangeState
Triggerall = Command = "a"
Triggerall = Command != "holddown"
Trigger1 = ctrl = 1
Trigger1 = StateType = A
Value = 600

;AA2
[State -1,AA2]
Type = ChangeState
Triggerall = Command = "x"
Triggerall = Command != "holddown"
triggerall = power >= 500
Trigger1 = ctrl = 1
Trigger1 = StateType = A
Value = 610




;---------------------------------------------------------------------------
;Kunai
[State -1]
Type = ChangeState
Triggerall = Command = "y"
Triggerall = Command != "holddown"
Trigger1 = ctrl = 1
Trigger1 = StateType = S
Value = 500

;c2 a2
[State -1]
type = ChangeState
value = 503
triggerall = stateno = 500
triggerall = command = "y"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = time >= 21


;---------------------------------------------------------------------------
;Harem
[State -1]
Type = ChangeState
Triggerall = Command = "start"
Triggerall = Command != "holddown"
triggerall = power >= 1000
Trigger1 = ctrl = 1
Trigger1 = StateType = S
Value = 1300




;---------------------------------------------------------------------------
;Kunai
[State -1]
Type = ChangeState
Triggerall = Command = "a"
Trigger1 = ctrl = 1
Trigger1 = StateType = C
Value = 400




;---------------------------------------------------------------------------
;Kunai
[State -1]
Type = ChangeState
Triggerall = Command = "y"
Triggerall = Command != "holddown"
Trigger1 = ctrl = 1
Trigger1 = StateType = A
Value = 560


;---------------------------------------------------------------------------
;Kunai
[State -1]
Type = ChangeState
Triggerall = Command = "down_y"
triggerall = power >= 1000
Trigger1 = ctrl = 1
Trigger1 = StateType = C
Value = 50002

;---------------------------------------------------------------------------
;Kunai
[State -1]
Type = ChangeState
Triggerall = Command = "b"
Triggerall = Command != "holddown"
triggerall = power >= 1000
Trigger1 = ctrl = 1
Trigger1 = StateType = A
Value = 5602

;---------------------------------------------------------------------------
;Rasengan
[State -1]
Type = ChangeState
Triggerall = Command = "c"
Triggerall = Command != "holddown"
triggerall = power >= 3000
Trigger1 = ctrl = 1
Trigger1 = StateType = S
Value = 789

;-----------------------------------------------------------------
;Kage Bunshin
[state -1, Kage Bunshin no Jutsu]
type = ChangeState
value = 7999
triggerall = numhelper(20000) < 1
triggerall = power >= 2000
triggerall = command = "down_b"
trigger1 = statetype = C
trigger1 = ctrl




;-----------------------------------------------------------------
;Kage Bunshin
[state -1, Kage Bunshin no Jutsu]
type = ChangeState
value = 8999
triggerall = numhelper(20000) < 1
triggerall = power >= 3000
triggerall = command = "down_c"
trigger1 = statetype = C
trigger1 = ctrl
;---------------------------------------------------------------------------
;Rendan
[State -1]
Type = ChangeState
Triggerall = Command = "b"
Triggerall = Command != "holddown"
triggerall = power >= 1000
Trigger1 = ctrl = 1
Trigger1 = StateType = S
Value = 983


;---------------------------------------------------------------------------
;Sasuke
[State -1]
Type = ChangeState
Triggerall = Command = "z"
Triggerall = Command != "holddown"
triggerall = power >= 3000
triggerall = life <= 450
Trigger1 = ctrl = 1
Trigger1 = StateType = S
Value = 7892








;-| AI 2|--------------------------------------------------------------

[Statedef -1]

[State -1, Activate AI]
type = VarSet
trigger1 = command = "CPU1"
trigger2 = command = "CPU2"
trigger3 = command = "CPU3"
trigger4 = command = "CPU4"
trigger5 = command = "CPU5"
trigger6 = command = "CPU6"
trigger7 = command = "CPU7"
trigger8 = command = "CPU8"
trigger9 = command = "CPU9"
trigger10 = command = "CPU10"
trigger11 = command = "CPU11"
trigger12 = command = "CPU12"
trigger13 = command = "CPU13"
trigger14 = command = "CPU14"
trigger15 = command = "CPU15"
v = 30 ;You can change this to any number provided that it doesn't collide with other variables.
value = 1






; ==========================
; AI Standing Guard
; ==========================
[State -1]
type = ChangeState
triggerall = var(30) = 1 ;AI trigger used
triggerall = Statetype != A ;Player is not in the air
triggerall = P2statetype != C ;Player is not crouching
triggerall = Statetype = S ;Player is currently standing
triggerall = P2Movetype = A ;Opponent is attacking
triggerall = Pos Y != [-1,-999]
triggerall = ctrl = 1
trigger1 = random <= 800 ;triggers at 80% probability
value = 130 ;Default standing guard state

; =============================
; AI Stand to Crouch Guard Transition
; =============================
[State -1]
type = ChangeState
triggerall = var(30) = 1
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
[State -1]
type = ChangeState
triggerall = var(30) = 1
triggerall = StateType != A
triggerall = P2statetype = C
triggerall = P2Movetype = A
triggerall = Pos Y != [-1,-999]
triggerall = ctrl = 1
trigger1 = random <= 800
value = 131

; =============================
; AI Crouch to Stand Guard Transition
; =============================
[State -1]
type = ChangeState
triggerall = var(30) = 1
triggerall = Statetype != A
triggerall = P2statetype != C
triggerall = P2Movetype = A
trigger1 = 1
trigger1 = stateno = 152
value = 150

; =============================
; AI Aerial Guard
; =============================
[State -1]
type = ChangeState
triggerall = var(30) = 1
triggerall = Statetype = A
triggerall = P2Movetype = A
triggerall = ctrl = 1
trigger1 = random <= 800
value = 132






;------------------------------------
;shiruken 1
;------------------------------------
[State -1]
type = ChangeState
triggerall = Var(30) = 1
triggerall = Statetype != A
triggerall = p2life != 0
triggerall = p2dist x >= 50
trigger1 = Ctrl = 1
trigger1 = Random <= 300
value = 500

;------------------------------------
;shiruken 2
;------------------------------------
[State -1]
type = ChangeState
triggerall = Var(30) = 1
triggerall = Statetype != A
triggerall = p2life != 0
triggerall = p2movetype = H
triggerall = stateno = 500
trigger1 = Ctrl = 1
trigger1 = time >= 21
value = 503




;---------------------------------------------------------------------------
;Stand Light Punch
;立ち弱パンチ
[State -1, Stand Light Punch]
type = ChangeState
value = 200
triggerall = Var(30) = 1
triggerall = Statetype != A
triggerall = p2life != 0
triggerall = p2movetype != A
triggerall = (p2dist x <= 100) && (p2dist x > 40)
trigger1 = statetype = S
trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Stand Light Punch
;立ち弱パンチ
[State -1, Stand Light Punch]
type = ChangeState
value = 210
triggerall = Var(30) = 1
triggerall = Statetype != A
triggerall = p2life != 0
triggerall = (stateno = 200) && (time >= 9)
triggerall = p2movetype = H
trigger1 = statetype = S

;---------------------------------------------------------------------------
;Stand Light Punch
;立ち弱パンチ
[State -1, Stand Light Punch]
type = ChangeState
value = 230
triggerall = Var(30) = 1
triggerall = Statetype != A
triggerall = p2life != 0
triggerall = (stateno = 210) && (time >= 9)
trigger1 = statetype = S


;---------------------------------------------------------------------------
;Stand Light Punch
;立ち弱パンチ
[State -1, Stand Light Punch]
type = ChangeState
value = 240
triggerall = Var(30) = 1
triggerall = Statetype != A
triggerall = p2life != 0
triggerall = p2movetype != A
triggerall = (p2dist x <= 95) && (p2dist x > 41)
trigger1 = statetype = S
trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Stand Light Punch
;立ち弱パンチ
[State -1, Stand Light Punch]
type = ChangeState
value = 250
triggerall = Var(30) = 1
triggerall = Statetype != A
triggerall = p2life != 0
triggerall = (stateno = 240) && (time >= 10)
triggerall = p2movetype = H
trigger1 = statetype = S

;---------------------------------------------------------------------------
;Stand Light Punch
;立ち弱パンチ
[State -1, Stand Light Punch]
type = ChangeState
value = 260
triggerall = Var(30) = 1
triggerall = Statetype != A
triggerall = p2life != 0
triggerall = (stateno = 250) && (time >= 16)
triggerall = p2movetype = H
trigger1 = statetype = S

;---------------------------------------------------------------------------
;Rendan
;立ち強キック
[State -1, ai Rendan]
type = ChangeState
value = 983
triggerall = Var(30) = 1
triggerall = Statetype != A
triggerall = p2life != 0
triggerall = power >= 1000
triggerall = random <= 700
triggerall = p2dist x <= 70




;---------------------------------------------------------------------------
;Stand Light Punch
;立ち弱パンチ
[State -1, Stand Light Punch]
type = ChangeState
value = 505
triggerall = Var(30) = 1
triggerall = p2life != 0
triggerall = movetype = H

;---------------------------------------------------------------------------
;Bunshin
;立ち強キック
[State -1, ai Bunshin]
type = ChangeState
value = 7999
triggerall = Var(30) = 1
triggerall = Statetype != A
triggerall = p2life != 0
triggerall = power >= 2000
triggerall = random <= 700
triggerall = p2dist x >= 80


;---------------------------------------------------------------------------
;Rasengan
;立ち強キック
[State -1, ai Rasengan]
type = ChangeState
value = 789
triggerall = Var(30) = 1
triggerall = Statetype != A
triggerall = p2life != 0
triggerall = power >= 3000
triggerall = random <= 700
triggerall = p2dist x >= 90



;---------------------------------------------------------------------------
;Harem
;立ち強キック
[State -1, ai Harem]
type = ChangeState
value = 1300
triggerall = Var(30) = 1
triggerall = Statetype != A
triggerall = p2life != 0
triggerall = power >= 1000
triggerall = random <= 700
triggerall = p2dist x >= 70



;---------------------------------------------------------------------------
;Air special
;立ち強キック
[State -1, ai Kunai]
type = ChangeState
value = 50002
triggerall = Var(30) = 1
triggerall = Statetype = A
triggerall = p2life != 0
triggerall = power >= 1000
triggerall = random <= 700
triggerall = p2dist x >= 50





;---------------------------------------------------------------------------
;Air special
;立ち強キック
[State -1, ai Kunai]
type = ChangeState
value = 5602
triggerall = Var(30) = 1
triggerall = Statetype = A
triggerall = p2life != 0
triggerall = power >= 1000
triggerall = random <= 900
triggerall = p2dist x >= 50








