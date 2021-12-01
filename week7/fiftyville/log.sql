-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Initial info: The theft took place on July 28, 2020 and that it took place on Chamberlin Street.

-- 1. Start with crime scene reports 
SELECT * FROM crime_scene_reports
WHERE 
    street LIKE '%Chamberlin%'
    AND year = 2020
    AND month = 7
    AND day = 28

-- Obtained info:
-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. 
-- Interviews were conducted today with three witnesses who were present at the time — 
-- each of their interview transcripts mentions the courthouse.

-- 2. Examine the interviews of the three witnesses 
SELECT * FROM interviews
WHERE 
    year = 2020
    AND month >= 7
    AND transcript LIKE '%courthouse%' 

-- Obtained info:
-- 161|Ruth|2020|7|28|Sometime within ten minutes of the theft, 
-- I saw the thief get into a car in the courthouse parking lot and drive away. 
-- If you have security footage from the courthouse parking lot, you might want 
-- to look for cars that left the parking lot in that time frame.

-- 3a. Looking up the security logs for 10:15-10:25 AM - finding the list of potential targets
SELECT 
    p.name
    , p.phone_number
    , p.passport_number
    , p.license_plate
FROM courthouse_security_logs csl
JOIN people p
    ON p.license_plate = csl.license_plate
WHERE 
    year = 2020
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute >= 15 
    AND minute <= 25

-- We got a list of license of potential targets:
-- Patrick  |(725) 555-4692|2963008352|5P2BI95
-- Ernest   |(367) 555-5533|5773159633|94KL13X
-- Amber    |(301) 555-4174|7526138472|6P58WS2
-- Danielle |(389) 555-5198|8496433585|4328GD8
-- Roger    |(130) 555-0289|1695452385|G412CB7
-- Elizabeth|(829) 555-5269|7049073643|L93JTIZ
-- Russell  |(770) 555-1861|3592750733|322W7JE
-- Evelyn   |(499) 555-9472|8294398571|0NTHK55

--162|Eugene|2020|7|28|I don't know the thief's name, but it was someone I recognized. 
-- Earlier this morning, before I arrived at the courthouse, I was walking by the ATM 
-- on Fifer Street and saw the thief there withdrawing some money.

-- 3b. Looking up atm transactions - narrowing the list down
WITH target_list AS (
    SELECT 
        p.id
        , p.name
        , p.phone_number
        , p.passport_number
        , p.license_plate
    FROM courthouse_security_logs csl
    JOIN people p
        ON p.license_plate = csl.license_plate
    WHERE 
        year = 2020
        AND month = 7
        AND day = 28
        AND hour = 10
        AND minute >= 15 
        AND minute <= 25)
SELECT 
    p.name
    , p.phone_number
    , p.passport_number
    , p.license_plate
    , ba.account_number
FROM atm_transactions atm
JOIN bank_accounts ba
    ON ba.account_number = atm.account_number
JOIN people p
    ON ba.person_id = p.id
WHERE 
    atm_location LIKE '%Fifer%'
    AND year = 2020
    AND month = 7
    AND day = 28
    AND transaction_type = 'withdraw'
    AND p.id IN (SELECT id FROM target_list)

-- 4 potential tergets left 
-- Ernest   |(367) 555-5533|5773159633|94KL13X|49610011
-- Russell  |(770) 555-1861|3592750733|322W7JE|26013199
-- Elizabeth|(829) 555-5269|7049073643|L93JTIZ|25506511
-- Danielle |(389) 555-5198|8496433585|4328GD8|28500762

--163|Raymond|2020|7|28|As the thief was leaving the courthouse, they called someone 
-- who talked to them for less than a minute. In the call, I heard the thief say that 
-- they were planning to take the earliest flight out of Fiftyville tomorrow. The thief 
-- then asked the person on the other end of the phone to purchase the flight ticket.

-- 3c narrowing the list down more by phone call tracking 
SELECT *
FROM 
    phone_calls
WHERE
    year = 2020 AND month = 7 AND day = 28
    AND duration < 60
    AND caller IN ('(367) 555-5533', '(770) 555-1861',
                   '(829) 555-5269', '(389) 555-5198')

-- It's either Ernest or Russell
-- 233|(367) 555-5533|(375) 555-8161|2020|7|28|45
-- 255|(770) 555-1861|(725) 555-3243|2020|7|28|49

-- 4.Find out the caller by flight search

SELECT 
    a.city
    , a2.city -- destination city
    , f.hour
    , p.passport_number
    , pe.name
FROM passengers p
JOIN flights f
    ON p.flight_id = f.id
JOIN airports a
    ON f.origin_airport_id = a.id
JOIN airports a2
    ON f.destination_airport_id = a2.id
JOIN people pe
    ON pe.passport_number = p.passport_number
WHERE 
    year = 2020 AND month = 7 AND day = 29
    AND hour < 12 -- Early flight
    AND p.passport_number IN ('5773159633','3592750733')

-- The thief is Ernest and he flew to London

-- 5. Finding the accomplice
SELECT 
    pc.receiver
    , p.name
FROM 
    phone_calls pc 
JOIN people p
    ON p.phone_number = pc.receiver
WHERE pc.id = 233