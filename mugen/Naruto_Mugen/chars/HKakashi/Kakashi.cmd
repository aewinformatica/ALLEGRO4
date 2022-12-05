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

[Command]
name = "AI41"
command = D,D,D,F,F,F,a+b+c+x+y+z,a
time = 1

[Command]
name = "AI42"
command = D,D,D,F,F,U,a+b+c+x+y+z,b
time = 1

[Command]
name = "AI43"
command = D,D,D,F,F,UF,a+b+c+x+y+z,c
time = 1

[Command]
name = "AI44"
command = D,D,D,F,F,D,a+b+c+x+y+z,x
time = 1

[Command]
name = "AI45"
command = D,D,D,F,F,DF,a+b+c+x+y+z,y
time = 1

[Command]
name = "AI46"
command = D,D,D,F,F,B,a+b+c+x+y+z,z
time = 1

[Command]
name = "AI47"
command = D,D,D,F,F,DB,a+b+c+x+y+z,a+b
time = 1

[Command]
name = "AI48"
command = D,D,D,F,F,UB,a+b+c+x+y+z,a+c
time = 1

[Command]
name = "AI49"
command = D,D,D,F,U,F,a+b+c+x+y+z,b+c
time = 1

[Command]
name = "AI50"
command = D,D,D,F,UF,F,a+b+c+x+y+z,x+z
time = 1

;-| Super Motions |--------------------------------------------------------
;The following two have the same name, but different motion.
;Either one will be detected by a "command = TripleKFPalm" trigger.
;Time is set to 20 (instead of default of 15) to make the move
;easier to do.

;Mangekyou Sharingan
[Command]
name = "MangekyouSharingan"
command = D, D, z
time = 20

;Summon NinDogs
[Command]
name = "NinDogs"
command = D, F, z
time = 20

;Raikiri
[Command]
name = "Raikiri"
command = D, B, z
time = 20

;Suiton: Suiryudan no Jutsu
[Command]
name = "Suiryudan"
command = D, B, c
time = 20

;Suiton: Daibakufu no Jutsu
[Command]
name = "Daibakufu"
command = D, D, c
time = 20

;Gather Chakra/Read
[Command]
name = "GatherChakra"
command = /$x
time = 1

;Hatake Kakashi Rendan
[Command]
name = "BunshinRendan"
command = F, D, z
time = 25

;Sennen Goroshi
[Command]
name = "Goroshi"
command = D, F, c
time = 20

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
;Punch
[State -1, Punch]
Type = ChangeState
Value = 200
Triggerall = Command = "a"
Triggerall = Command != "holddown"
Trigger1 = StateType = S
Trigger1 = ctrl = 1
Trigger2 = StateNo = 430
Trigger2 = AnimElemTime(3) > 0

;---------------------------------------------------------------------------
;Slash and Kick
[State -1, Slash and Kick]
Type = ChangeState
Value = 225
Triggerall = Command = "y"
Triggerall = Command != "holddown"
Trigger1 = StateType = S
Trigger1 = ctrl = 1
Trigger2 = StateNo = 200
Trigger2 = AnimElemTime(4) > 0

;---------------------------------------------------------------------------
;Raikiri
[State -1, Raikiri]
Type = ChangeState
Value = 375
Triggerall = Command = "Raikiri" && Power >= 3000
Triggerall = Command != "holddown"
Trigger1 = StateType = S
Trigger1 = ctrl = 1
Trigger2 = StateNo = 400
Trigger2 = AnimElemTime(7) > 0

;---------------------------------------------------------------------------
;Summon Large Dog
[State -1, Summon Large Dog]
Type = ChangeState
Value = 250
Triggerall = Command = "b"
Triggerall = Command != "holddown"
Trigger1 = StateType = S
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Summon Pa-kun
[State -1, Summon Pa-kun]
Type = ChangeState
Value = 275
Triggerall = Command = "b"
Triggerall = Command = "holddown"
Trigger1 = StateType = C
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Air Kunai I
[State -1, Air Kunai I]
Type = ChangeState
Value = 325
Triggerall = Command = "b"
Triggerall = Command != "holddown"
Trigger1 = StateType = A
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Air Kunai II
[State -1, Air Kunai II]
Type = ChangeState
Value = 350
Triggerall = Command = "x"
Triggerall = Command != "holddown"
Trigger1 = StateType = A
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Mangekyou Sharingan
[State -1, Mangekyou Sharingan]
Type = ChangeState
Value = 900
Triggerall = Command = "MangekyouSharingan" && Power >= 4000
Triggerall = Command != "holddown"
Trigger1 = StateType = S
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Summon Dogs
[State -1, Summon Nindogs]
Type = ChangeState
Value = 400
Triggerall = Command = "NinDogs" && Power >= 1000
Triggerall = Command != "holddown"
Triggerall = p2StateType != A
Trigger1 = StateType = S
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Bunshin Attack I
[State -1, Bunshin Attack I]
Type = ChangeState
Value = 420
Triggerall = Command = "a"
Triggerall = Command = "holddown"
Trigger1 = StateType = C
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Bunshin Attack II
[State -1, Bunshin Attack II]
Type = ChangeState
Value = 426
Triggerall = Command = "y"
Triggerall = Command = "holddown"
Trigger1 = StateType = C
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Air Dog Summon
[State -1, Air Summon Nindog]
Type = ChangeState
Value = 450
Triggerall = Command = "a"
Triggerall = Command != "holddown"
Trigger1 = StateType = A
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Suiton: Suiryudan no Jutsu
[State -1, Suiton: Suiryudan no Jutsu]
Type = ChangeState
Value = 725
Triggerall = Command = "Suiryudan" && power >= 1500
Triggerall = Command != "holddown"
Trigger1 = StateType = S
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Suiton: Daibakufu no Jutsu
[State -1, Suiton: Daibakufu no Jutsu]
Type = ChangeState
Value = 800
Triggerall = Command = "Daibakufu" && Power >= 2000
Triggerall = Command != "holddown"
Trigger1 = StateType = S
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Reading/Gathering Chakra
[State -1, Reading/Gathering Chakra]
Type = ChangeState
Value = 700
Triggerall = Command = "GatherChakra" && power < 4000
Triggerall = Command = "holddown"
Trigger1 = StateType = C
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Air Slash
[State -1, Air Slash]
Type = ChangeState
Value = 950
Triggerall = Command = "y"
Triggerall = Command != "holddown"
Trigger1 = StateType = A
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Hatake Kakashi Rendan
[State -1, Hatake Kakashi Rendan]
type = ChangeState
Value = 1100
Triggerall = Command = "BunshinRendan" && Power >= 1250
Triggerall = Command != "holddown"
Trigger1 = StateType = S
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Shiruken Throw I
[State -1, Shiruken Throw I]
Type = ChangeState
Value = 1225
Triggerall = Command = "x"
Triggerall = Command != "holddown"
Trigger1 = StateType = S
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Shiruken Throw II
[State -1, Shiruken Throw II]
Type = ChangeState
Value = 1226
Triggerall = Command = "x"
Triggerall = Command != "holddown"
Trigger1 = StateType = S
Trigger1 = ctrl = 1
Trigger2 = StateNo = 1225
Trigger2 = AnimElemTime(6) > 0 && AnimElemTime(7) < 0

;---------------------------------------------------------------------------
;Sennen Goroshi
[State -1, Sennen Goroshi]
Type = ChangeState
Value = 3125
Triggerall = Command = "Goroshi" && Power >= 750
Triggerall = Command != "holddown"
Trigger1 = StateType = S
Trigger1 = ctrl = 1

;---------------------------------------------------------------------------
;Kawarimi
[State -1, Kawarimi]
Type = ChangeState
Value = 650
Triggerall = Command = "c" && power >= 200
Triggerall = StateType != A
Trigger1 = StateNo = 5000
Trigger2 = StateNo = 5001
Trigger3 = StateNo = 5010
Trigger4 = StateNo = 5011
Trigger5 = StateNo = 5020
Trigger6 = StateNo = 5030
Trigger7 = StateNo = 5035
Trigger8 = StateNo = 5040
Trigger9 = StateNo = 5050
Trigger10 = StateNo = 5070
Trigger11 = StateNo = 5071

;---------------------------------------------------------------------------
;Kawarimi + Explosive Tag
[State -1, Kawarimi + Explosive Tag]
Type = ChangeState
Value = 500
Triggerall = Command = "z" && power >= 500
Triggerall = StateType != A
Trigger1 = StateNo = 5000
Trigger2 = StateNo = 5001
Trigger3 = StateNo = 5010
Trigger4 = StateNo = 5011
Trigger5 = StateNo = 5020
Trigger6 = StateNo = 5030
Trigger7 = StateNo = 5035
Trigger8 = StateNo = 5040
Trigger9 = StateNo = 5050
Trigger10 = StateNo = 5070
Trigger11 = StateNo = 5071

;----------------------------------------------------------------------------
;AI Activation
[State -1, AI Activation]
type = VarSet
triggerall = var(50) != 1
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
trigger41 = command = "AI41"
trigger42 = command = "AI42"
trigger43 = command = "AI43"
trigger44 = command = "AI44"
trigger45 = command = "AI45"
trigger46 = command = "AI46"
trigger47 = command = "AI47"
trigger48 = command = "AI48"
trigger49 = command = "AI49"
trigger50 = command = "AI50"
var(50) = 1
