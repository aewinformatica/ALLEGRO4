[Command]
name = "AI1"
command = D,DF,a+b+a+b+a+b
time = 1

[Command]
name = "AI2"
command = D,D,a+b+a+b+a+b
time = 1

[Command]
name = "AI3"
command = D,a+b+a+b+a+b
time = 1

[Command]
name = "AI4"
command = D,D,a+b+a+b+a+b
time = 1

[Command]
name = "AI5"
command = D,D,a+b+a+b+a+b
time = 1

[Command]
name = "AI6"
command = D,B,a+b+a+b+a+b
time = 1

[Command]
name = "AI7"
command = D,D,a+b+a+b+a+b
time = 1

[Command]
name = "AI8"
command = D,D,a+a+b+a+b+a+b
time = 1

[Command]
name = "AI9"
command = D,D,a+b+a+b+a+b
time = 1

[Command]
name = "AI10"
command = D,D,a+b+a+b+a+b
time = 1

[Command]
name = "AI11"
command = a+b+a+b+a+x
time = 1

[Command]
name = "AI12"
command = a+b+a+b+a+y
time = 1

[Command]
name = "AI13"
command = a+b+a+b+a+a
time = 1

[Command]
name = "AI14"
command = a+b+a+b+a+b
time = 1

[Command]
name = "AI15"
command = a+b+a+b+a+x
time = 1

[Command]
name = "AI16"
command = a+b+a+b+a+y
time = 1

[Command]
name = "AI17"
command = a+b+a+b+a+b
time = 1

[Command]
name = "AI18"
command = a+b+a+b+a+b
time = 1

[Command]
name = "AI19"
command = a+b+a+b+a+b
time = 1

[Command]
name = "AI20"
command = a+b+a+b+a+b
time = 1

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


; Don't remove the following line. It's required by the CMD standard.
[Statedef -1]

[State -1, Katon: Housenka no Jutsu]
type = ChangeState
value = 620
triggerall = command = "b"
triggerall = Statetype = A
triggerall = power > 899
trigger1 = ctrl
;----------------------------------------------------------------------------
[State -1, Katon: Goukakyou no Jutsu]
type = ChangeState
value = 1000
triggerall = command = "x"
triggerall = command = "holddown"
triggerall = Statetype = C
triggerall = power > 899
trigger1 = ctrl
;-----------------------------------------------------------------------------
;Kawarimi
[State -1, kawarimi]
type = ChangeState
value = 666
triggerall = command = "z"
triggerall = power >= 0
trigger1 = stateno = 5000
trigger2 = stateno = 5001
trigger3 = stateno = 5002
trigger4 = stateno = 5010
trigger5 = stateno = 5011
trigger6 = stateno = 5012
trigger7 = stateno = 5020
trigger8 = stateno = 5021
trigger9 = stateno = 5022
;-----------------------------------------------------------------------------
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
;----------------------------------------------------------------------------
[State -1, Activate AI]
type = VarSet
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
trigger10 = command = "AI11"
trigger10 = command = "AI12"
trigger10 = command = "AI13"
trigger10 = command = "AI14"
trigger10 = command = "AI15"
trigger10 = command = "AI16"
trigger10 = command = "AI17"
trigger10 = command = "AI18"
trigger10 = command = "AI19"
trigger10 = command = "AI20"
var(59) = 1
value = 1
;------------------------------------------------------------------------------
;sharingan?
[state -1]
type = changestate
triggerall = command = "c"
trigger1 = ctrl && statetype != A
trigger1 = power >= 800
value = 3000
;------------------------------------------------------------------------------
[State -1]
type = ChangeState
value = 1020
triggerall = command = "b"
triggerall = command != "holddown"
triggerall = statetype = S
triggerall = power > 1999
trigger1 = ctrl = 1

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
;Stand Light Punch
[State -1]
Type = ChangeState
Triggerall = Command = "a"
Triggerall = Command != "holddown"
Trigger1 = ctrl
Trigger1 = StateType = S
Value = 200

;----------------------------------------------------------------------------
;Stand Light Kick
[State -1]
Type = ChangeState
Triggerall = Command = "a"
Triggerall = Command != "holddown"
Trigger1 = ctrl
Trigger1 = StateType = S
Value = 210
trigger2 = stateno = 200
trigger2 = animelemtime(3) > 0

;------------------------------------------------------------------------------
;Standing Hard Punch
[State -1]
Type = ChangeState
Triggerall = Command = "a"
Triggerall = Command != "holddown"
Trigger1 = ctrl
Trigger1 = StateType = S
Value = 220
trigger2 = stateno = 210
trigger2 = animelemtime(3) > 0

;------------------------------------------------------------------------------
;Standing Hard Kick
[State -1]
Type = ChangeState
Triggerall = Command = "a"
Triggerall = Command != "holddown"
Trigger1 = ctrl
Trigger1 = StateType = S
Value = 230
trigger2 = stateno = 220
trigger2 = animelemtime(4) > 0

;------------------------------------------------------------------------------
; Dash Punch
[State -1]
type = ChangeState
value = 300
triggerall = command = "a"
triggerall = command != "holddown"
triggerall = Stateno = 100
trigger1 = ctrl

;------------------------------------------------------------------------------
;Air Kick
[State -1]
type = ChangeState
value = 600
triggerall = command = "a"
triggerall = command != "holddown"
triggerall = Statetype = A
trigger1 = ctrl = 1
;------------------------------------------------------------------------------
;Teleport
;[State -1]
;type = ChangeState
;value = 400
;triggerall = command = "y"
;triggerall = command = "holddown"
;triggerall = Statetype = C
;trigger1 = ctrl = 1
;------------------------------------------------------------------------------
[State -1]
type = ChangeState
value = 410
triggerall = command = "a"
triggerall = command = "holddown"
triggerall = Statetype = C
trigger1 = ctrl = 1
;------------------------------------------------------------------------------
[State -1, Kunai]
type = ChangeState
value = 201
triggerall = command = "y"
triggerall = statetype = S
trigger1 = ctrl
;------------------------------------------------------------------------------
[State -1, Air Kunai]
type = ChangeState
value = 202
triggerall = command = "y"
triggerall = statetype = A
trigger1 = ctrl
;------------------------------------------------------------------------------
;Stand Light Kick
[State -1]
Type = ChangeState
Triggerall = Command = "x"
Triggerall = Command != "holddown"
Trigger1 = ctrl
Trigger1 = StateType = S
Value = 210
;------------------------------------------------------------------------------
;Standing Hard Kick
[State -1]
Type = ChangeState
Triggerall = Command = "x"
Triggerall = Command != "holddown"
Trigger1 = ctrl
Trigger1 = StateType = S
Value = 230
trigger2 = stateno = 210
trigger2 = animelemtime(3) > 0
;------------------------------------------------------------------------------
[State -1]
type = ChangeState
value = 410
triggerall = command = "x"
triggerall = command != "holddown"
triggerall = Statetype = S
trigger1 = ctrl = 1
trigger2 = stateno = 230
trigger2 = animelemtime(3) > 0
;------------------------------------------------------------------------------
