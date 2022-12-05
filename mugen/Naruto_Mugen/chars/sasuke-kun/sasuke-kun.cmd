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

;----- inicio dos comandos da AI -----------------------------------------------------

[Command]
name = "AI_1"
command = D,D,D,F,F,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI_2"
command = D,D,D,F,F,U,a+b+c+x+y+z
time = 1

[Command]
name = "AI_3"
command = D,D,D,F,F,UF,a+b+c+x+y+z
time = 1

[Command]
name = "AI_4"
command = D,D,D,F,F,D,a+b+c+x+y+z
time = 1

[Command]
name = "AI_5"
command = D,D,D,F,F,DF,a+b+c+x+y+z
time = 1

[Command]
name = "AI_6"
command = D,D,D,F,F,B,a+b+c+x+y+z
time = 1

[Command]
name = "AI_7"
command = D,D,D,F,F,DB,a+b+c+x+y+z
time = 1

[Command]
name = "AI_8"
command = D,D,D,F,F,UB,a+b+c+x+y+z
time = 1

[Command]
name = "AI_9"
command = D,D,D,F,U,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI_10"
command = D,D,D,F,UF,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI_11"
command = D,D,D,F,DF,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI_12"
command = D,D,D,F,D,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI_13"
command = D,D,D,F,DB,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI_14"
command = D,D,D,F,B,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI_15"
command = D,D,D,F,UB,F,a+b+c+x+y+z
time = 1

[Command]
name = "AI_16"
command = D,D,D,F,F,F,a+b+c+x+y,z
time = 1

[Command]
name = "AI_17"
command = D,D,D,F,F,F,a+b+c+x,y,z
time = 1

[Command]
name = "AI_18"
command = D,D,D,F,F,F,a+b+c,x,y,z
time = 1

;[Command]
;name = "AI_19"
;command = D,D,D,F,F,F,a+b,c,x,y,z
;time = 1

;[Command]
;name = "AI_20"
;command = D,D,D,F,F,F,a,b,c,x,y,z
;time = 1

;[Command]
;name = "AI_21"
;command = D,D,D,F,F,F,a+b+c,x+y+z
;time = 1

;--------------------- fim AI comandos ------------------------------------


;-| Super Motions |--------------------------------------------------------
;The following two have the same name, but different motion.
;Either one will be detected by a "command = TripleKFPalm" trigger.
;Time is set to 20 (instead of default of 15) to make the move
;easier to do.

[Command]
name = "super_pulo"
command = ~D,U
time = 20

[Command]
name = "TripleKFPalm"   ;Same name as above
command = ~D, DB, B, a
time = 20

[Command]
name = "TripleKFPalm"   ;Same name as above
command = ~D, DB, B, b
time = 20

[Command]
name = "TripleKFPalm"   ;Same name as above
command = ~D, DB, B, c
time = 20

[Command]
name = "naruto"
command = a+x

[Command]
name = "z+c"
command = z+c

[Command]
name = "b+c"
command = b+c

;-| Special Motions |------------------------------------------------------
[Command]
name = "upper_x"
command = ~D, DF, F, a

[Command]
name = "upper_y"
command = ~D, DF, F, b

[Command]
name = "upper_xy"
command = ~F, D, DF, a+b

[Command]
name = "QCF_x"
command = ~D, DF, F, x

[Command]
name = "QCF_y"
command = ~D, DF, F, y

[Command]
name = "QCB_z"
command = ~D, DB, B, z

[Command]
name = "QCF_z"
command = ~D, DF, F, z

[Command]
name = "QCF_xy"
command = ~D, DF, F, x+y

[Command]
name = "QCB_x"
command = ~D, DB, B, x

[Command]
name = "QCB_y"
command = ~D, DB, B, y

[Command]
name = "QCB_xy"
command = ~D, DB, B, x+y

[Command]
name = "FF_ab"
command = y+b

[Command]
name = "FF_a"
command = F, F, a

[Command]
name = "FF_b"
command = F, F, b

[Command]
name = "FF_c"
command = ~D, DF, F, c

[Command]
name = "D+a"
command = ~D, DF, F, a;D,a

[Command]
name = "D+b"
command = ~D, DF, F, b;D,b

[Command]
name = "D+c"
command = ~D, DF, F, c;D,c

[Command]
name = "D+x"
command = ~D, DF, F, x;D,x

[Command]
name = "D+y"
command = ~D, DF, F, y;D,y

[Command]
name = "D+z"
command = ~D, DF, F, z;D,z

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

;----------------- ativação da AI --------------------------
[State -1]
type = VarSet
trigger1 = command = "AI_1"
trigger2 = command = "AI_2"
trigger3 = command = "AI_3"
trigger4 = command = "AI_4"
trigger5 = command = "AI_5"
trigger6 = command = "AI_6"
trigger7 = command = "AI_7"
trigger8 = command = "AI_8"
trigger9 = command = "AI_9"
trigger10 = command = "AI_10"
trigger11 = command = "AI_11"
trigger12 = command = "AI_12"
trigger13 = command = "AI_13"
trigger14 = command = "AI_14"
trigger15 = command = "AI_15"
trigger16 = command = "AI_16"
trigger17 = command = "AI_17"
trigger18 = command = "AI_18"
;trigger19 = command = "AI_19"
;trigger20 = command = "AI_20"
;trigger21 = command = "AI_21"
v = 59
value = 1
;----------------------------------------------------------------------
;----------- AI defesa ------------------------------------------------
;1 AI defesa ----------------------------------------------------------
[State -1]
type = ChangeState
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 =  P2BodyDist X <= 75 && Random <= 600
trigger1 = Ctrl && P2MoveType = A && StateType = S
value = 130

;--------------------- defesa abaixado ----------------------------------
;2 AI defesa abaixado ---------------------------------------------------
[State -1]
type = ChangeState
value = 152
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = P2BodyDist X <= 75 && Random <= 600
trigger1 = Ctrl && P2MoveType = A && StateType = C
triggerall = P2statetype = C
;-------------------------------------------------------------------------
;2 kunai
[State -1]
type = ChangeState
value = 1010
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && StateType = S
trigger1 = Random <= 600
;-------------------------------------------------------------------------
;---------------------------------------------------------------------------
;4 AI selo nivel 3
[State -1]
type = ChangeState
value = 1120
triggerall = power >= 1000
triggerall = life <= 130
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && StateType = S
trigger1 = Random <= 800
;---------------------------------------------------------------------------
;5 AI estrela grande
[State -1]
type = ChangeState
value = 1020
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && StateType = S
trigger1 = Random <= 600
;---------------------------------------------------------------------------
;15 AI gatu pequeno
[State -1]
type = ChangeState
value = 3054
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && StateType = S
trigger1 = Random <= 600
;---------------------------------------------------------------------------
;6 gatu
[State -1]
type = ChangeState
value = 1210
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && StateType = S
trigger1 = Random <= 600
;--------------------------------------------------------------------------
;7 chidori rapido ----------------------------------------------------
[State -1]
type = ChangeState
value = 1248
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && statetype = S
trigger1 = Random <= 600
;-----------------------------------------------------------------------
;---------------------------------------------------------------------------
;8 AI rendan
[State -1]
type = ChangeState
value = 1220
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && StateType = S
trigger1 = Random <= 600
;---------------------------------------------------------------------------
;9 AI kawarimi
[State -1]
type = ChangeState
value = 195
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && P2MoveType = A && statetype != A
trigger1 = Random <= 700
;------------------------------------------------------------------------
;10 kunai z
[State -1]
type = ChangeState
value = 1012
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && StateType = S
trigger1 = Random <= 600
;---------------------------------------------------------------------------
;17 AI selo nivel 2
[State -1, selo nivel 2]
type = ChangeState
value = 1400
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && StateType = S
trigger1 = Random <= 600
;triggerall = power >= 500
;triggerall = life <= 500
trigger1 = var(1) ;Use combo condition (above)
;--------------------------------------------------------------------------
;11 pulo para tras
[State -1, pulo para tras ]
type = ChangeState
value = 1016
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && StateType = S
trigger1 = Random <= 600
;------------------------------------------------------------------------
;12 AI super chute a--------------------------------------------------------
[State -1]
type = ChangeState
value = 1050
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && StateType = S
trigger1 = Random <= 600
;-------------------------------------------------------------------------
;13 AI shidori
[State -1]
type = ChangeState
value = 1250
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Random <= 600
trigger1 = Ctrl && statetype != A
;---------------------------------------------------------------------------
;14 AI subindo com chute
[State -1]
type = ChangeState
value = 1100
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = P2BodyDist X <= 65 && Random <= 700
trigger1 = Ctrl && StateType = S
;--------------------------------------------------------------------------
;16 estrela grande
[State -1]
type = ChangeState
value = 1014
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && StateType = S
trigger1 = P2BodyDist X <= 120 && Random <= 700
;---------------------------------------------------------------------------
;18 AI especial chute
[State -1]
type = ChangeState
value = 1110
trigger1 = RoundState = 2 && Var(59) != 0
trigger1 = Ctrl && StateType = S
trigger1 = P2BodyDist X <= 97 && Random <= 700
;---------------------------------------------------------------------------

;----------------------------------------------------------------------------

;----------------------------------------------------------------------------

;-------------- fim da AI ---------------------------------------------------

;----------------------------------------------------------------------------

;===========================================================================
;This is not a move, but it sets up var(1) to be 1 if conditions are right
;for a combo into a special move (used below).
;Since a lot of special moves rely on the same conditions, this reduces
;redundant logic.
[State -1, Combo condition Reset]
type = VarSet
trigger1 = 1
var(1) = 0

[State -1, Combo condition Check]
type = VarSet
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = (stateno = [200,299]) || (stateno = [400,499])
trigger2 = stateno != 440 ;Except for sweep kick
trigger2 = movecontact
var(1) = 1
;--------------------------------------------------------------------------
;gatu no ar
[State -1, gatu no ar ]
type = ChangeState
value = 10185
triggerall = command = "D+x" || command = "D+y" || command = "D+z"
triggerall = !var(0)
trigger1 = statetype = A
trigger1 = ctrl = !0
;trigger2 = movecontact
;---------------------------------------------------------------------------
;super voadora
[State -1, super voadora ]
type = ChangeState
value = 10160
triggerall = command = "D+c" || command = "D+b" || command = "D+a"
triggerall = !var(0)
trigger1 = statetype = A
trigger1 = ctrl = !0
;trigger2 = movecontact
;-----------------------------------------
;--------------------------------------------------------------------------
;shidori rapido)
[State -1, shidori rapido]
type = ChangeState
value = 1249;3000
triggerall = command = "TripleKFPalm"
trigger1 = statetype = S
trigger1 = var(1) ;Use combo condition (above)
;---------------------------------------------------------------------------
;kakashi
;[State -1, kakashi]
;type = ChangeState
;value = 1070
;triggerall = command = "FF_ab"
;triggerall = power >= 3000
;triggerall = numhelper(1071) = 0
;triggerall = power >= 330
;trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;super chute a
[State -1, super chute a]
type = ChangeState
value = 1050
triggerall = command = "FF_a"
trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;super chute b
[State -1, super chute b]
type = ChangeState
value = 1060
triggerall = command = "FF_b"
trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;super chute c
[State -1, super chute c]
type = ChangeState
value = 1060
triggerall = command = "FF_c"
trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;selo
[State -1, selo]
type = ChangeState
value = 1400;1020
triggerall = command = "QCF_xy"
triggerall = power >= 3000;330
trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;gatu pequeno
[State -1, gatu pequeno]
type = ChangeState
value = 3054
triggerall = command = "QCF_x"
trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;kunai
[State -1, kunai]
type = ChangeState
value = 1010
triggerall = command = "QCF_y"
trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;kunai z
[State -1, kunai z]
type = ChangeState
value = 1012
triggerall = command = "QCF_z"
trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;estrela grande
[State -1, estrela grande]
type = ChangeState
value = 1020
triggerall = command = "QCB_z"
trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;pulo para tras
[State -1, pulo para tras ]
type = ChangeState
value = 1016
triggerall = command = "b+c"
trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;chacra
[State -1,carregando chacra]
type = ChangeState
value = 1090
triggerall = command = "z+c";"QCF_yz"
trigger1 = ctrl = 1
triggerall = power >= 10
trigger1 = var(1) ;Use combo condition (above)
;---------------------------------------------------------------------------
;selo nivel 3
[State -1, selo nivel 3]
type = ChangeState
value = 1120
triggerall = command = "upper_xy"
triggerall = power >= 3000
trigger1 = statetype = S
trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;subindo com chute
[State -1, subindo com chute]
type = ChangeState
value = 1100
triggerall = command = "upper_x"
trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;especial chute
[State -1, especial chute]
type = ChangeState
value = 1110
triggerall = command = "upper_y"
trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;rendan
[State -1, rendan]
type = ChangeState
value = 1220
triggerall = command = "QCB_xy"
triggerall = power >= 330
trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------

;naruto
;[State -1, naruto]
;type = ChangeState
;value = 1080
;triggerall = power >= 3000
;triggerall = command = "naruto"
;triggerall = numhelper(1081) = 0
;trigger1 = statetype != A

;---------------------------------------------------------------------------
;shidori
[State -1, shidori]
type = ChangeState
value = 1250
triggerall = power >= 3000
triggerall = command = "QCB_x"
trigger1 = statetype != A
trigger1 = ctrl
;trigger1 = var(1) ;Use combo condition (above)

;---------------------------------------------------------------------------
;gatu
[State -1, gatu]
type = ChangeState
value = 1210
triggerall = command = "QCB_y"
trigger1 = ctrl
trigger1 = var(1) ;Use combo condition (above)

;===========================================================================
;super pulo
[State -1, super pulo]
type = ChangeState
value = 1300
triggerall = command = "super_pulo"
trigger1 = (statetype = s) && ctrl

;---------------------------------------------------------------------------
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
;arremessando o p2
[State -1, arremessando o p2]
type = ChangeState
value = 194;800
triggerall = command = "y"
triggerall = statetype = S
triggerall = ctrl
triggerall = stateno != 100
trigger1 = command = "holdfwd"
trigger1 = p2bodydist X < 3
trigger1 = (p2statetype = S) || (p2statetype = C)
trigger1 = p2movetype != H
trigger2 = command = "holdback"
trigger2 = p2bodydist X < 5
trigger2 = (p2statetype = S) || (p2statetype = C)
trigger2 = p2movetype != H


;===========================================================================
;---------------------------------------------------------------------------
;Stand Punch  x
[State -1, Stand Punch x]
type = ChangeState
value = 200
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 200
trigger2 = time > 6

;---------------------------------------------------------------------------
;Stand Punch y
[State -1, Stand Punch y]
type = ChangeState
value = 210
triggerall = command = "y"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = (stateno = 200) && time > 5
trigger3 = (stateno = 230) && time > 6

;---------------------------------------------------------------------------
;Stand Punch z
[State -1, Stand Punch z]
type = ChangeState
value = 215
triggerall = command = "z"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = (stateno = 200) && time > 5
trigger3 = (stateno = 230) && time > 6

;---------------------------------------------------------------------------
;Stand Kick a
[State -1, Stand Kick a]
type = ChangeState
value = 280
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = (stateno = 200) && time > 7
trigger3 = (stateno = 230) && time > 9

;---------------------------------------------------------------------------
;Standing Kick b
[State -1, Stand Kick b]
type = ChangeState
value = 240
triggerall = command = "b"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = (stateno = 200) && time > 5
trigger3 = (stateno = 230) && time > 6

;---------------------------------------------------------------------------
[State -1, Stand Kick c]
type = ChangeState
value = 250
triggerall = command = "c"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = (stateno = 200) && time > 5
trigger3 = (stateno = 230) && time > 6

;---------------------------------------------------------------------------
;kawarimi
[State -1, kawarimi]
type = ChangeState
value = 195
triggerall = command = "start"
trigger1 = statetype != A
trigger1 = ctrl

;---------------------------------------------------------------------------
;Crouching Punch x
[State -1, Crouching Punch x]
type = ChangeState
value = 400
triggerall = command = "x"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl

;---------------------------------------------------------------------------
;Crouching Punch y
[State -1, Crouching Punch y]
type = ChangeState
value = 410
triggerall = command = "y"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = (stateno = 400) || (stateno = 430)
trigger2 = (time > 9) || (movecontact && time > 5)

;---------------------------------------------------------------------------
;Crouching Punch z
[State -1, Crouching Punch z]
type = ChangeState
value = 420
triggerall = command = "z"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = (stateno = 400) || (stateno = 430)
trigger2 = (time > 9) || (movecontact && time > 5)

;---------------------------------------------------------------------------
;Crouching Kick a
[State -1, Crouching Kick a]
type = ChangeState
value = 430
triggerall = command = "a"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = (stateno = 400) || (stateno = 430)
trigger2 = (time > 9) || (movecontact && time > 5)

;---------------------------------------------------------------------------
;Crouching Kick b
[State -1, Crouching Kick b]
type = ChangeState
value = 440
triggerall = command = "b"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = (stateno = 400) || (stateno = 430)
trigger2 = (time > 9) || (movecontact && time > 5)

;---------------------------------------------------------------------------
;Crouching Kick c
[State -1, Crouching Kick c]
type = ChangeState
value = 450
triggerall = command = "c"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = (stateno = 400) || (stateno = 430)
trigger2 = (time > 9) || (movecontact && time > 5)

;---------------------------------------------------------------------------
;Jump Punch x
[State -1, Jump Punch x]
type = ChangeState
value = 600
triggerall = command = "x"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 600
trigger2 = statetime >= 7
trigger3 = stateno = 1350 ;Air blocking

;---------------------------------------------------------------------------
;Jump Punch y
[State -1, Jump Punch y]
type = ChangeState
value = 610
triggerall = command = "y"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 600 || stateno = 630 ;jump_x or jump_a
trigger2 = movecontact
trigger3 = stateno = 1350 ;Air blocking

;---------------------------------------------------------------------------
;Jump Punch z
[State -1, Jump Punch z]
type = ChangeState
value = 620
triggerall = command = "z"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 600 || stateno = 630 ;jump_x or jump_a
trigger2 = movecontact
trigger3 = stateno = 1350 ;Air blocking

;---------------------------------------------------------------------------
;Jump Kick a
[State -1, Jump Kick a]
type = ChangeState
value = 630
triggerall = command = "a"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 1350 ;Air blocking

;---------------------------------------------------------------------------
;Jump Kick b
[State -1, Jump Kick b]
type = ChangeState
value = 640
triggerall = command = "b"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 600 || stateno = 630 ;jump_x or jump_a
trigger2 = movecontact
trigger3 = stateno = 1350 ;Air blocking

;----------------------------------------------------------------------------
;Jump Kick c
[State -1, Jump Kick c]
type = ChangeState
value = 650
triggerall = command = "c"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 600 || stateno = 630 ;jump_x or jump_a
trigger2 = movecontact
trigger3 = stateno = 1350 ;Air blocking

;----------------------------------------------------------------------------

