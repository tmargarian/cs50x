WITH 
    jd_movies AS (
    SELECT 
        m.title, m.id
    FROM movies m
    JOIN stars s ON s.movie_id = m.id
    JOIN people p ON p.id = s.person_id
    WHERE p.name = 'Johnny Depp'
    ) 

SELECT 
    m.title
FROM jd_movies m
JOIN stars s
    ON s.movie_id = m.id
JOIN people p
    ON s.person_id = p.id
WHERE
    p.name = 'Helena Bonham Carter'