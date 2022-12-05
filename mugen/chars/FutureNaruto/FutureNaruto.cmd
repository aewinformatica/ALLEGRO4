;===========================================================================
;                           BUTTON REMAPPING
;===========================================================================

[Remap]
x = x
y = y
z = z
a = a
b = b
c = c
s = s

;===========================================================================
;                            DEFAULT VALUES
;===========================================================================

[Defaults]
command.time = 15

command.buffer.time = 1

;===========================================================================
;                              DOUBLE TAP
;===========================================================================
[Command]
name = "FF"
command = F, F
time = 10

[Command]
name = "BB"
command = B, B
time = 10

;===========================================================================
;                        2/3 BUTTON COMBINATION
;===========================================================================
[Command]
name = "recovery"
command = x+y
time = 1

;===========================================================================
;                         DIRECTION + BUTTON
;===========================================================================

[Command]
name = "down_a"
command = /$D,a
time = 1

[Command]
name = "down_b"
command = /$D,b
time = 1

;===========================================================================
;                           SINGLE BUTTON
;===========================================================================

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

;===========================================================================
;                          HOLD DIRECTION
;===========================================================================

[Command]
name = "holdfwd"
command = /$F
time = 1

[Command]
name = "holdback"
command = /$B
time = 1

[Command]
name = "holdup"
command = /$U
time = 1

[Command]
name = "holddown"
command = /$D
time = 1

;===========================================================================
;                           HOLD BUTTON
;===========================================================================

[Command]
name = "holda"
command = /a
time = 1

[Command]
name = "holdb"
command = /b
time = 1

[Command]
name = "holdc"
command = /c
time = 1

[Command]
name = "holdx"
command = /x
time = 1

[Command]
name = "holdy"
command = /y
time = 1

[Command]
name = "holdz"
command = /z
time = 1

[Command]
name = "holdstart"
command = /s
time = 1

;===========================================================================
;                           SPECIAL MOTIONS
;===========================================================================

[Command]
name = "F_x"
command = /F, x
time = 15

[Command]
name = "F_b"
command = /F, b
time = 10

[Command]
name = "F_a"
command = /F, a
time = 10

;===========================================================================
;                                CPU
;===========================================================================

[Command]
name = "CPU1"
command = D, D, U, U, D, U
time = 1

[Command]
name = "CPU2"
command = D, U, U, D, D, U
time = 1

[Command]
name = "CPU3"
command = D, D, D, U, U, U
time = 1

[Command]
name = "CPU4"
command = D, D, U, U, D, U
time = 1

[Command]
name = "CPU5"
command = D, U, U, D, D, U
time = 1

[Command]
name = "CPU6"
command = D, D, D, U, U, U
time = 1

[Command]
name = "CPU7"
command = D, D, U, U, D, U
time = 1

[Command]
name = "CPU8"
command = D, U, U, D, D, U
time = 1

[Command]
name = "CPU9"
command = D, D, D, U, U, U
time = 1

[Command]
name = "CPU10"
command = D, D, U, U, D, U
time = 1

[Command]
name = "CPU11"
command = D, U, U, D, D, U
time = 1

[Command]
name = "CPU12"
command = D, D, D, U, U, U
time = 1

[Command]
name = "CPU13"
command = D, D, U, U, D, U
time = 1

[Command]
name = "CPU14"
command = D, U, U, D, D, U
time = 1

[Command]
name = "CPU15"
command = D, D, D, U, U, U
time = 1

;===========================================================================
                             [Statedef -1]
;===========================================================================

;===========================================================================
;                             ACTIVATE AI
;===========================================================================

[State -1, Activate AI]
type = VarSet
trigger1 = command = "CPU1" || command = "CPU2" || command = "CPU3" || command = "CPU4" || command = "CPU5" || command = "CPU6" || command = "CPU7" || command = "CPU8" || command = "CPU9" || command = "CPU10" || command = "CPU11" || command = "CPU12"
trigger2 = command = "CPU13" || command = "CPU14" || command = "CPU15"
v = 3
value = 1



;___________________________________________________________________________
;___________________________________________________________________________
;___________________________________________________________________________



;===========================================================================
;===========================================================================
;                         REGULAR FORM COMMANDS
;===========================================================================
;===========================================================================


;===========================================================================
;                             RUN FORWARD
;===========================================================================

[State -1, Run Fwd]
type = ChangeState
value = 100
trigger1 = command = "FF"
trigger1 = statetype = S
trigger1 = ctrl



;===========================================================================
;                               RUN BACK
;===========================================================================

[State -1, Run Back]
type = ChangeState
value = 105
trigger1 = command = "BB"
trigger1 = statetype = S
trigger1 = ctrl



;===========================================================================
;                             HAREM NO JUTSU
;===========================================================================

;Harem no jutsu
[State -1, Harem no jutsu]
type = Changestate
value = 7000
triggerall = command = "start"
triggerall = command = "holddown"
triggerall = power >= 1000 && var(0) = 0 && statetype != A
trigger1 = statetype = C
trigger1 = ctrl = 1
trigger2 = movecontact
trigger2 = stateno = 200 || 210 || 230 || 240 || 410 || 360 || 430



;===========================================================================
;                            OIROIKE NO JUTSU
;===========================================================================

[State -1, Oiroike no jutsu]
type = Changestate
value = 6800
triggerall = command = "start"
triggerall = command != "holddown" && var(0) = 0 && statetype != A
trigger1 = statetype = S
trigger1 = ctrl = 1
triggerall = power >= 100



;===========================================================================
;                           AIR SHURIKEN THROW
;===========================================================================

[State -1, Air Shuriken Throw]
type = ChangeState
value = 6100
triggerall = command = "c"
triggerall = command != "holddown" && var(0) = 0 && statetype != S && statetype != C
trigger1 = ctrl
trigger1 = statetype = A



;===========================================================================
;                           NARUTO NINJA SPECIAL
;===========================================================================

[State -1, Naruto Ninja Special]
type = Changestate
value = 4400
triggerall = command = "F_x"
triggerall = power >= 2000 && var(0) = 0 && statetype != A
trigger1 = ctrl
trigger1 = statetype = S
trigger2 = movecontact
trigger2 = stateno = 360 || 200 || 210 || 230 || 240 || 410 || 430



;===========================================================================
;                             RASEN-SHURIKEN
;===========================================================================

[State -1, Rasen-Shuriken]
type = ChangeState
value = 370
triggerall = command = "z"
triggerall = command = "holddown" && var(0) = 0 && statetype != A
triggerall = power >= 3000
trigger1 = ctrl = 1
trigger1 = statetype = C



;===========================================================================
;                             OODAMA RASENGAN
;===========================================================================

[State -1, Oodama Rasengan]
type = ChangeState
value = 300
triggerall = command = "z"
triggerall = command != "holddown" && var(0) = 0 && statetype != A
triggerall = power >= 1000
trigger1 = statetype = S
trigger1 = ctrl = 1
triggerall = life <= 300



;===========================================================================
;                                 RASENGAN
;===========================================================================

[State -1, Rasengan]
type = ChangeState
value = 290
triggerall = command = "z"
triggerall = command != "holddown" && var(0) = 0 && statetype != A
triggerall = power >= 1000
trigger1 = statetype = S
trigger1 = ctrl = 1



;===========================================================================
;                                HARD PUNCH
;===========================================================================

[State -1, Hard Punch]
type = ChangeState
value = 350
triggerall = command = "F_b" && var(0) = 0 && statetype != A
trigger1 = statetype = S
trigger1 = ctrl = 1
trigger2 = movecontact
trigger2 = stateno = 210
trigger3 = movecontact
trigger3 = stateno = 410



;===========================================================================
;                                KUNAI SLICE
;===========================================================================

[State -1, Kunai Slice]
type = ChangeState
value = 360
triggerall = command = "a"
triggerall = command = "holddown" && var(0) = 0 && statetype != A
trigger1 = statetype = C
trigger1 = ctrl = 1
trigger2 = movecontact
trigger2 = stateno = 240



;===========================================================================
;                               LIGHT PUNCH
;===========================================================================

[State -1, Light Punch]
type= Changestate
value = 200
Triggerall = Command = "a"
Triggerall = Command != "holddown" && var(0) = 0 && statetype != A
Trigger1 = ctrl = 1
Trigger1 = StateType = S



;===========================================================================
;                               LIGHT PUNCH 2
;===========================================================================

[State -1, Light Punch2]
type= Changestate
value = 230
Triggerall = Command = "a"
Triggerall = Command != "holddown" && var(0) = 0 && statetype != A
Trigger1 = ctrl = 1
Trigger1 = StateType = S
Trigger2 = MoveContact
Trigger2 = stateno = 200



;===========================================================================
;                               HEAVY PUNCH
;===========================================================================

[State -1, Heavy Punch]
type= Changestate
value = 210
Triggerall = Command = "b"
Triggerall = Command != "holddown" && var(0) = 0 && statetype != A
Trigger1 = ctrl = 1
Trigger1 = StateType = S
Trigger2 = Movecontact
Trigger2 = stateno = 230



;===========================================================================
;                                HEADBUTT
;===========================================================================

[State -1, HeadButt]
type= Changestate
value = 240
Triggerall = Command = "b"
Triggerall = Command = "holddown" && var(0) = 0 && statetype != A
Trigger1 = ctrl = 1
Trigger1 = StateType = C
Trigger2 = MoveContact
Trigger2 = stateno = 210



;===========================================================================
;                                JUMP KICK
;===========================================================================

[State -1, Jump Kick]
type= ChangeState
value = 250
Triggerall = Command = "b"
Triggerall = Command != "holddown" && var(0) = 0 && statetype != A
Trigger1 = MoveContact
Trigger1 = stateno = 240



;===========================================================================
;                                LAUNCH KICK
;===========================================================================

[State -1, Launch Kick]
type = Changestate
value = 400
Triggerall = Command = "a"
Triggerall = Command != "holddown" && var(0) = 0 && statetype != A
Trigger1 = MoveContact
Trigger1 = stateno = 430



;===========================================================================
;                               KUNAI SLASH
;===========================================================================
;Kunai Slash
[State -1, Kunai Slash]
type = ChangeState
value = 410
triggerall = command = "a"
triggerall = command != "holddown" && var(0) = 0 && statetype != A
trigger1 = movecontact
trigger1 = stateno = 240
trigger2 = movecontact
trigger2 = stateno = 360



;===========================================================================
;                                TURN KICK
;===========================================================================

[State -1, Turn Kick]
type = ChangeState
value = 430
triggerall = command = "a"
triggerall = command != "holddown" && var(0) = 0 && statetype != A
trigger1 = movecontact
trigger1 = stateno = 410



;===========================================================================
;                                 AIR KICK
;===========================================================================

[State -1, Air Kick]
type = ChangeState
value = 600
triggerall = command = "a"
triggerall = command != "holddown" && var(0) = 0 && statetype != S && statetype != C
trigger1 = statetype = A
trigger1 = ctrl = 1



;===========================================================================
;                                MISSLE KICK
;===========================================================================

[State -1, Missle Kick]
type = ChangeState
value = 610
triggerall = command = "b"
triggerall = command != "holddown" && var(0) = 0 && statetype != S && statetype != C
trigger1 = statetype = A
trigger1 = ctrl = 1



;===========================================================================
;                             NO-ESCAPE SHURIKEN
;===========================================================================

[State -1, No-escape Shuriken]
type = Changestate
value = 6300
triggerall = command = "c"
triggerall = command = "holddown" && var(0) = 0
triggerall = Pos Y <= -60
triggerall = power >= 1000 && statetype != S && statetype != C
trigger1 = statetype = A
trigger1 = ctrl = 1



;===========================================================================
;                               SHURIKEN THROW
;===========================================================================

[State -1, Shuriken Throw]
type = ChangeState
value = 260
triggerall = command = "c"
triggerall = command != "holddown" && var(0) = 0 && statetype != A
trigger1 = statetype = S
trigger1 = ctrl = 1



;===========================================================================
;                               KAGE SHURIKEN
;===========================================================================

[State -1, Kage Shuriken]
type = ChangeState
value = 270
triggerall = command = "c"
triggerall = command = "holddown" && var(0) = 0 && statetype != A
triggerall = power >= 250
trigger1 = statetype = C
trigger1 = ctrl = 1



;===========================================================================
;                              NARUTO RENDAN
;===========================================================================
;Naruto Rendan
[State -1, Naruto Rendan]
type = ChangeState
value = 280
triggerall = command = "y"
triggerall = command != "holddown"
triggerall = power >= 1000 && var(0) = 0 && statetype != A
trigger1 = statetype = S
trigger1 = ctrl = 1
trigger2 = movecontact
trigger2 = stateno = 200
trigger3 = movecontact
trigger3 = stateno = 210
trigger4 = movecontact
trigger4 = stateno = 230
trigger5 = movecontact
trigger5 = stateno = 240
trigger6 = movecontact
trigger6 = stateno = 400
trigger7 = movecontact
trigger7 = stateno = 410
trigger8 = movecontact
trigger8 = stateno = 430



;===========================================================================
;                                  KAWARIMI
;===========================================================================

[State -1, Kawarimi]
type = ChangeState
value = 3700
triggerall = command = "x"
triggerall = command != "holddown" && var(0) = 0 && statetype != A
triggerall = power >= 1000
trigger1 = stateno = 5000
trigger2 = stateno = 5001
trigger3 = stateno = 5002
trigger4 = stateno = 5010
trigger5 = stateno = 5011
trigger6 = stateno = 5012
trigger7 = stateno = 5020
trigger8 = stateno = 5021
trigger9 = stateno = 5022



;===========================================================================
;                               KAGE BUNSHIN
;===========================================================================

[State -1, Kage Bunshin]
type = ChangeState
value = 4300
triggerall = command = "x"
triggerall = command != "holddown"
triggerall = numhelper(20000) < 1
triggerall = power >= 2000 && var(0) = 0 && statetype != A
trigger1 = ctrl
trigger1 = statetype = S


;===========================================================================
; GATHER CHAKRA
;===========================================================================

[State -1, Gather Chakra]
type = Changestate
value = 7300
triggerall = command = "holda" && command = "holdx" && power <= 2999 && var(0) = 0 && statetype != A && statetype != C
trigger1 = statetype = S && ctrl = 1



;===========================================================================
;                           KYUUBI TRANSFORMATION
;===========================================================================

[State -1, Kyuubi Tranformation]
type = Changestate
value = 7200
triggerall = command = "x"
triggerall = command = "holddown" && statetype != A
triggerall = power >= 3000
triggerall = life <= 500
trigger1 = statetype = C && ctrl = 1 && var(0) = 0



;===========================================================================
; EVASIVE DODGE
;===========================================================================

[State -1, Evasive Dodge]
type = ChangeState
value = 7600
triggerall = command = "holdy" && command = "holdb" && var(0) = 0 && statetype != A
trigger1 = statetype = S && ctrl = 1 || statetype = C && ctrl = 1 || stateno = 20 || stateno = 100



;___________________________________________________________________________
;___________________________________________________________________________
;___________________________________________________________________________




;===========================================================================
;===========================================================================
;                             KYUUBI FORM COMMANDS
;===========================================================================
;===========================================================================


;===========================================================================
;                               KYUUBI SCRATCH
;===========================================================================

[State -1, Kyuubi Scratch]
type = ChangeState
value = 10300
triggerall = command = "a"
triggerall = command != "holddown" && statetype != A && var(0) = 1
trigger1 = statetype = S && ctrl = 1 || stateno = 10500 && movecontact



;===========================================================================
;                           KYUUBI UNDERHAND SLASH
;===========================================================================

[State -1, Underhand Slash]
type = ChangeState
value = 10350
triggerall = command = "a"
triggerall = command != "holddown" && statetype != A && var(0) = 1
trigger1 = stateno = 10300 && movecontact



;===========================================================================
;                                  KYUUBI CLAW
;===========================================================================

[State -1, Kyuubi Claw]
type = ChangeState
value = 10400
trigger1 = command = "a"
trigger1 = command != "holddown" && statetype != A && var(0) = 1
trigger1 = stateno = 10350 && movecontact
trigger2 = stateno = 10500 && movecontact && command = "F_a" && command != "holddown"



;===========================================================================
;                              KYUUBI TAIL WHIP
;===========================================================================

[State -1, Tail Whip]
type = ChangeState
value = 10450
triggerall = command = "b"
triggerall = command != "holddown" && statetype != A && var(0) = 1
trigger1 = statetype = S && ctrl = 1 || stateno = 10350 && movecontact



;===========================================================================
;                              KYUUBI HEADBUTT
;===========================================================================

[State -1, HeadButt]
type = ChangeState
value = 10500
triggerall = command = "b"
triggerall = command != "holddown" && statetype != A && var(0) = 1
trigger1 = stateno = 10450 && movecontact



;===========================================================================
;                           KYUUBI SLASH FRENZY 1
;===========================================================================

[State -1, Slash Frenzy 1]
type = ChangeState
value = 10550
trigger1 = command = "b"
trigger1 = command != "holddown" && statetype != A && var(0) = 1
trigger1 = stateno = 10500 && movecontact
trigger2 = stateno = 10350 && movecontact && command = "F_b" && command != "holddown"



;===========================================================================
;                          KYUUBI SLASH FRENZY 2
;===========================================================================

[State -1, Slash Frenzy 2]
type = ChangeState
value = 10600
triggerall = command = "b"
triggerall = command != "holddown" && statetype != A && var(0) = 1
trigger1 = animelemtime(8) >= 1



;===========================================================================
;                          KYUUBI AIR TAIL WHIP
;===========================================================================

[State -1, Air Tail Whip]
type = ChangeState
value = 10700
triggerall = command = "a" && command != "holddown" && statetype != S && statetype != C && var(0) = 1
trigger1 = statetype = A && ctrl = 1



;===========================================================================
;                         KYUUBI MISSLE HEADBUTT
;===========================================================================

[State -1, Missle HeadButt]
type = ChangeState
value = 10750
triggerall = command = "b" && command != "holddown" && statetype != S && statetype != C && var(0) = 1
trigger1 = statetype = A && ctrl = 1



;===========================================================================
; KYUUBI SCREAM
;===========================================================================

[State -1, Kyuubi Scream]
type = ChangeState
value = 10800
triggerall = command = "start" && command != "holddown" && statetype != A && var(0) = 1 && life >= 30
trigger1 = statetype = S && ctrl = 1



;===========================================================================
; KYUUBI UPPER SLASH
;===========================================================================

[State -1, Upper Slash]
type = ChangeState
value = 10850
triggerall = command = "a" && command = "holddown" && statetype != A && var(0) = 1
trigger1 = statetype = C && ctrl = 1 || stateno = 10350 && movecontact || stateno = 10500 && movecontact



;___________________________________________________________________________
;___________________________________________________________________________
;___________________________________________________________________________



;===========================================================================
;===========================================================================
;                       REGULAR NARUTO AI COMMANDS
;===========================================================================
;===========================================================================

;===========================================================================
;                       ( AI ) OIROIKE NO JUTSU
;===========================================================================

[State -1, ( AI )Oiroike no jutsu]
type = ChangeState
value = 6800
triggerall = var(3) = 1 && statetype != A && statetype != C && MoveType != H && statetype != L && P2Life != 0 && var(0) = 0 && power >= 100 && prevstateno != 6800
trigger1 = P2Dist X <= 150 && P2Dist y >= -10 && ctrl = 1 && statetype = S && random >= 551 && random <= 600
trigger2 = P2Dist X >= 151 && p2dist X <= 400 && ctrl = 1 && statetype = S && random >= 181 && random <= 270
trigger3 = p2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 201 && random <= 300 && p2movetype = A
trigger4 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -10 && statetype = S && ctrl = 1 && random <= 333 && p2movetype = A


;===========================================================================
;                         ( AI ) LIGHT PUNCH
;===========================================================================

[State -1, ( AI )Light Punch]
type = Changestate
value = 200
triggerall = var(3) = 1 && statetype != A && statetype != C && MoveType  != H && statetype != L && var(0) = 0
trigger1 = P2Dist X <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random <= 190
trigger2 = P2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random <= 333 && p2movetype = H
trigger3 = p2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random <= 100 && p2movetype = A



;===========================================================================
;                        ( AI ) LIGHT PUNCH 2
;===========================================================================

[State -1, ( AI ) Light Punch 2]
type = ChangeState
value = 230
triggerall = var(3) = 1 && statetype != A && statetype != C && MoveType != H && statetype != L && var(0) = 0
trigger1 = P2Dist X <= 150 && p2dist y >= -10 && stateno = 200 && movecontact



;===========================================================================
;                         ( AI ) HEAVY PUNCH
;===========================================================================

[State -1, ( AI ) Heavy Punch]
type = ChangeState
value = 210
triggerall = var(3) = 1 && statetype != A && MoveType != H && statetype != L && var(0) = 0
trigger1 = P2Dist X <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 191 && random <= 380
trigger2 = p2dist x <= 150 && p2dist y >= -10 && stateno = 230 && movecontact



;===========================================================================
;                          ( AI ) HARD PUNCH
;===========================================================================

[State -1, ( AI ) Hard Punch]
type = ChangeState
value = 350
triggerall = var(3) = 1 && statetype != A && MoveType != H && statetype != L && var(0) = 0
trigger1 = P2Dist X <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 381 && random <= 450
trigger2 = p2dist x <= 150 && p2dist y >= -10 && movecontact && stateno = 210 && random <= 250



;===========================================================================
;                           ( AI ) HEADBUTT
;===========================================================================
[State -1, ( AI ) HeadButt]
type = ChangeState
value = 240
triggerall = var(3) = 1 && statetype != A && MoveType != H && statetype != L && var(0) = 0
trigger1 = P2Dist X <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 451 && random <= 500
trigger2 = P2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 334 && random <= 666 && p2movetype = H
trigger3 = p2dist x <= 150 && p2dist y >= -10 && movecontact && stateno = 210 && random >= 251 && random <= 500



;===========================================================================
;                         ( AI ) JUMP KICK
;===========================================================================

[State -1, ( AI ) Jump Kick]
type = ChangeState
value = 250
triggerall = var(3) = 1 && statetype != A && MoveType != H && statetype != L && var(0) = 0
trigger1 = P2Dist X <= 150 && p2dist y >= -10 && stateno = 240 && movecontact && random <= 500



;===========================================================================
;                           ( AI ) RUN BACK
;===========================================================================

[State -1, ( AI ) Run Back]
type = ChangeState
value = 105
triggerall = var(3) = 1 && statetype != A && MoveType != H && statetype != L && var(0) = 0
trigger1 = P2Dist X <= 151 && P2Dist Y >= -10 && ctrl = 1 && statetype = S && random >= 501 && random <= 550



;===========================================================================
; ( AI ) RUN FORWARD
;===========================================================================

[State -1, ( AI ) Run Forward]
type = ChangeState
value = 100
triggerall = var(3) = 1 && statetype != A && movetype != H && statetype != L && var(0) = 0
trigger1 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 91 && random <= 180
trigger2 = p2dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 181 && random <= 270



;===========================================================================
;                         ( AI ) KUNAI SLASH
;===========================================================================

[State -1, ( AI ) Kunai Slash]
type = ChangeState
value = 410
triggerall = var(3) = 1 && statetype != A && MoveType != H && statetype != L && var(0) = 0
trigger1 = p2dist x <= 150 && p2dist y >= -10 && movecontact && stateno = 360 && random <= 750



;===========================================================================
;                           ( AI ) KUNAI SLICE
;===========================================================================

[State -1, ( AI ) Kunai Slice]
type = ChangeState
value = 360
triggerall = var(3) = 1 && statetype != A && movetype != H && statetype != L && var(0) = 0
trigger1 = P2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = C && random <= 600
trigger2 = p2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 101 && random <= 200 && p2movetype = A
trigger3 = p2dist x <= 150 && p2dist y >= -10 && movecontact && stateno = 240 && random >= 501



;===========================================================================
;                           ( AI ) TURN KICK
;===========================================================================

[State -1, ( AI ) Turn Kick]
type = ChangeState
value = 430
triggerall = var(3) = 1 && statetype != A && movetype != H && statetype != L && var(0) = 0
trigger1 = P2Dist X <= 150 && p2dist y >= -10 && stateno = 410 && movecontact && random <= 500



;===========================================================================
;                          ( AI ) LAUNCH KICK
;===========================================================================

[State -1, ( AI ) Launch Kick]
type = ChangeState
value = 400
triggerall = var(3) = 1 && statetype != A && movetype != H && statetype != L && var(0) = 0
trigger1 = P2Dist X <= 150 && p2dist y >= -10 && stateno = 430 && movecontact && random <= 500



;===========================================================================
;                          ( AI ) GATHER CHAKRA
;===========================================================================

[State -1, ( AI ) Gather Chakra]
type = ChangeState
value = 7300
triggerall = var(3) = 1 && statetype != A && movetype != H && statetype != L && power <= 2999 && var(0) = 0
trigger1 = P2Dist X >= 151 && p2dist x <= 400 && p2dist y >= -10 && statetype = S && ctrl = 1 && random <= 90
trigger2 = p2dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 91 && random <= 180



;===========================================================================
;                            ( AI ) KAWARIMI
;===========================================================================
[State -1, ( AI ) Kawarimi]
type = ChangeState
value = 3700
triggerall = var(3) = 1 && statetype != A && movetype = H && statetype != L && power >= 1000 && random <= 500 && var(0) = 0
trigger1 = stateno = 5000
trigger2 = stateno = 5001
trigger3 = stateno = 5002
trigger4 = stateno = 5010
trigger5 = stateno = 5011
trigger6 = stateno = 5012
trigger7 = stateno = 5020
trigger8 = stateno = 5021
trigger9 = stateno = 5022



;===========================================================================
;                           ( AI ) AIR KICK
;===========================================================================

[State -1, ( AI ) Air Kick]
type = ChangeState
value = 600
triggerall = var(3) = 1 && statetype != S && statetype != C && movetype != H && statetype != L && var(0) = 0
trigger1 = p2dist x <= 151 && p2dist y >= -10 && p2dist y <= 0 && ctrl = 1 && statetype = A && random <= 600



;===========================================================================
;                       ( AI ) MISSLE KICK
;===========================================================================

[State -1, ( AI ) Missle Kick]
type = ChangeState
value = 610
triggerall = var(3) = 1 && statetype != S && statetype != C && movetype != H && statetype != L && var(0) = 0 && P2StateType != L
trigger1 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -200 && ctrl = 1 && statetype = A && random <= 300



;===========================================================================
;                    ( AI ) NO-ESCAPE SHURIKEN
;===========================================================================

[State -1, ( AI ) No-Escape Shuriken]
type = ChangeState
value = 6300
triggerall = var(3) = 1 && statetype != S && statetype != C && movetype != H && statetype != L && var(0) = 0 && power >= 1000
trigger1 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -200 && ctrl = 1 && statetype = A && random >= 601 && random <= 999
trigger2 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -200 && ctrl = 1 && statetype = A && random >= 681 && random <= 820 && p2statetype = L



;===========================================================================
;                          ( AI ) RASENGAN
;===========================================================================

[State -1, ( AI ) Rasengan]
type = ChangeState
value = 290
triggerall = var(3) = 1 && var(0) = 0 && statetype != A && movetype != H && statetype != L && power >= 1000 && life >= 301
trigger1 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 271 && random <= 360
trigger2 = p2dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 271 && random <= 360
trigger3 = p2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 201 && random <= 400 && p2statetype = L
trigger4 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -10 && statetype = S && ctrl = 1 && random <= 170 && p2statetype = L



;===========================================================================
;                        ( AI ) NARUTO RENDAN
;===========================================================================

[State -1, ( AI ) Naruto Rendan]
type = ChangeState
value = 280
triggerall = var(3) = 1 && var(0) = 0 && statetype != A && movetype != H && statetype != L && power >= 1000
trigger1 = P2Dist X <= 150 && P2Dist y >= -10 && ctrl = 1 && statetype = S && random >= 731 && random <= 780
trigger2 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -10 && statetype = S && ctrl = 1 && random >= 561 && random <= 670
trigger3 = p2dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 541 && random <= 610
trigger4 = p2dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random <= 499 && p2movetype = H
trigger5 = p2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 301 && random <= 400 && p2movetype = A
trigger6 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 334 && random <= 666 && p2movetype = A
trigger7 = p2dist x <= 150 && p2dist y >= -10 && movehit && stateno = 210 && random >= 501 && random <= 750



;===========================================================================
;                       ( AI ) OODAMA RASENGAN
;===========================================================================

[State -1, ( AI ) Oodama Rasengan]
type = ChangeState
value = 300
triggerall = var(3) = 1 && var(0) = 0 && statetype != A && movetype != H && statetype != L && power >= 1000 && life <= 300
trigger1 = P2Dist X >= 151 && P2Dist X <= 400 && P2Dist Y <= -10 && ctrl = 1 && random >= 360 && random <= 450 && statetype = S
trigger2 = P2Dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 361 && random <= 450
trigger3 = p2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 401 && random <= 600 && p2statetype = L
trigger4 = p2dist x >= 151 && p2dist x <= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 171 && random <= 340 && p2statetype = L



;===========================================================================
;                     ( AI ) NARUTO NINJA SPECIAL
;===========================================================================

[State -1, ( AI ) Naruto Ninja Special]
type = ChangeState
value = 4400
triggerall = var(0) = 0 && var(3) = 1 && statetype != A && movetype != H && statetype != L && power >= 2000 && Pos Y = 0
trigger1 = P2Dist X <= 150 && P2Dist Y >= -10 && ctrl = 1 && statetype = S && random >= 611 && random <= 680
trigger2 = p2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 667 && random <= 999 && p2movetype = H
trigger3 = p2dist x <= 150 && p2dist y >= -10 && moveguarded && stateno = 210 && random >= 751 && random <= 999



;===========================================================================
;                        ( AI ) SHURIKEN THROW
;===========================================================================

[State -1, ( AI ) Shuriken Throw]
type = ChangeState
value = 260
triggerall = var(0) = 0 && var(3) = 1 && statetype != A && movetype != H && statetype != L
trigger1 = p2dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random <= 90
trigger2 = p2dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random <= 600 && p2movetype = A



;===========================================================================
;                       ( AI ) AIR SHURIKEN THROW
;===========================================================================

[State -1, ( AI ) Air Shuriken Throw]
type = ChangeState
value = 6100
triggerall = var(0) = 0 && var(3) = 1 && statetype != S && statetype != C && movetype != H && statetype != L
trigger1 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -200 && statetype = S && ctrl = 1 && random >= 821 && random <= 999 && p2statetype = L



;===========================================================================
;                          ( AI ) KAGE SHURIKEN
;===========================================================================
[State -1, ( AI ) Kage Shuriken]
type = ChangeState
value = 270
triggerall = var(0) = 0 && var(3) = 1 && statetype != A && movetype != H && statetype != L && power >= 250
trigger1 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -200 && ctrl = 1 && statetype = A && random >= 301 && random <= 600



;===========================================================================
;                          ( AI ) KAGE BUNSHIN
;===========================================================================
[State -1, ( AI ) Kage Bunshin]
type = ChangeState
value = 4300
triggerall = var(0) = 0 && var(3) = 1 && statetype != A && movetype != H && statetype != L && power >= 2000
trigger1 = P2Dist X <= 150 && P2Dist Y >= -10 && ctrl = 1 && statetype = S && random >= 781 && random <= 870
trigger2 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -10 && ctrl = 1 && random >= 671 && random <= 760
trigger3 = p2dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 611 && random <= 720
trigger4 = p2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 601 && random <= 800 && p2statetype = L
trigger5 = p2dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random <= 500 && p2statetype = L
trigger6 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 341 && random <= 510 && p2statetype = L



;===========================================================================
; ( AI ) CROUCH
;===========================================================================

[State -1, ( AI ) Crouch]
type = ChangeState
value = 11
triggerall = var(0) = 0 && var(3) = 1 && statetype != A && movetype != H && statetype != L
trigger1 = P2Dist X <= 150 && P2Dist Y >= -10 && ctrl = 1 && statetype = S && random >= 871 && random <= 940
trigger2 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 761 && random >= 850
trigger3 = p2dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 721 && random <= 810



;===========================================================================
;                         ( AI ) HAREM NO JUTSU
;===========================================================================

[State -1, ( AI ) Harem no Jutsu]
type = ChangeState
value = 7000
triggerall = var(0) = 0 && var(3) = 1 && statetype != A && movetype != H && statetype != L && power >= 1000
trigger1 = P2Dist X <= 150 && P2Dist Y >= -10 && ctrl = 1 && statetype = S && random >= 601 && random <= 610
trigger2 = p2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random <= 200 && p2statetype = L



;===========================================================================
;                         ( AI ) RASEN-SHURIKEN
;===========================================================================

[State -1, ( AI ) Rasen-Shuriken]
type = ChangeState
value = 370
triggerall = var(0) = 0 && var(3) = 1 && statetype != A && movetype != H && statetype != L && power >= 3000
trigger1 = p2dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 451 && random <= 540



;===========================================================================
;                      ( AI ) KYUUBI TRANSFORMATION
;===========================================================================
[State -1, ( AI ) Kyuubi Transformation]
type = ChangeState
value = 7300
triggerall = var(0) = 0 && var(3) = 1 && statetype != A && movetype != H && statetype != L && power >= 3000 && life <= 500
trigger1 = P2Dist X <= 150 && P2Dist Y >= -10 && ctrl = 1 && statetype = S && random >= 941 && random <= 999
trigger2 = P2dist x >= 151 && p2dist x <= 400 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 851 && random <= 999
trigger3 = P2dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 811 && random <= 999
trigger4 = p2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 801 && random <= 999 && p2statetype = L
trigger5 = p2dist x >= 401 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 501 && random <= 999 && p2statetype = L
trigger6 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 511 && random <= 680 && p2statetype = L



;===========================================================================
; ( AI ) EVASIVE DODGE
;===========================================================================
[State -1, ( AI ) Evasive Dodge]
type = ChangeState
value = 7600
triggerall = var(0) = 0 && var(3) = 1 && statetype != A && movetype != H && statetype != L
trigger1 = P2Dist X <= 150 && P2Dist Y >= -10 && ctrl = 1 && statetype = S && random >= 681 && random <= 730
trigger2 = P2Dist X >= 151 && p2dist x <= 400 && p2dist y >= -10 && statetype = S && ctrl = 1 && random >= 451 && random <= 560
trigger3 = p2dist x <= 150 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 401 && random <= 999 && p2movetype = A
trigger4 = p2dist x >= 151 && p2dist x <= 400 && p2dist y >= -10 && ctrl = 1 && statetype = S && random >= 664 && random <= 99 && p2movetype = A
trigger5 = p2dist x <= 150 && random <= 333 && stateno = 100 && statetype = S



;___________________________________________________________________________
;___________________________________________________________________________
;___________________________________________________________________________



;===========================================================================
;===========================================================================
;                       KYUUBI FORM AI COMMANDS
;===========================================================================
;===========================================================================



;-


;___________________________________________________________________________
;___________________________________________________________________________
;___________________________________________________________________________



;===========================================================================
;===========================================================================
;                              MISC. AI
;===========================================================================
;===========================================================================


; ==========================================================================
;                        ( AI ) Standing Guard
; ==========================================================================

[State -1, ( AI ) Standing Guard]
type = ChangeState
triggerall = var(3) = 1
triggerall = Statetype != A
triggerall = P2statetype != C
triggerall = Statetype = S
triggerall = P2Movetype = A
triggerall = Pos Y != [-1,-999]
triggerall = ctrl = 1
trigger1 = random <= 800
value = 130



; ==========================================================================
;                 ( AI ) Stand to Crouch Guard Transition
; ==========================================================================

[State -1, ( AI ) Stand to Crouch Guard]
type = ChangeState
triggerall = var(3) = 1
triggerall = StateType != A
triggerall = P2statetype = C
triggerall = P2Movetype = A
triggerall = Pos Y != [-1,-999]
trigger1 = stateno = 150
trigger1 = 1
value = 152



; ==========================================================================
;                        ( AI ) Crouching Guard
; ==========================================================================

[State -1, ( AI ) Crouch Guard]
type = ChangeState
triggerall = var(3) = 1
triggerall = StateType != A
triggerall = P2statetype = C
triggerall = P2Movetype = A
triggerall = Pos Y != [-1,-999]
triggerall = ctrl = 1
trigger1 = random <= 800
value = 131



; ==========================================================================
;                 ( AI ) Crouch to Stand Guard Transition
; ==========================================================================

[State -1, ( AI ) Crouch to Stand Guard]
type = ChangeState
triggerall = var(3) = 1
triggerall = Statetype != A
triggerall = P2statetype != C
triggerall = P2Movetype = A
trigger1 = 1
trigger1 = stateno = 152
value = 150



; ==========================================================================
;                        ( AI ) Aerial Guard
; ==========================================================================

[State -1, ( AI ) Aerial Guard]
type = ChangeState
triggerall = var(3) = 1
triggerall = Statetype = A
triggerall = P2Movetype = A
triggerall = ctrl = 1
trigger1 = random <= 800
value = 132


