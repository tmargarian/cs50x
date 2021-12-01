WITH 
    kb_movies AS (
    SELECT 
        m.title, m.id
    FROM movies m
    JOIN stars s ON s.movie_id = m.id
    JOIN people p ON s.person_id = p.id
    WHERE p.name = 'Kevin Bacon'
        AND p.birth = 1958
    )

SELECT DISTINCT 
    p.name
FROM kb_movies m
JOIN stars s ON s.movie_id = m.id
JOIN people p ON s.person_id = p.id
WHERE name <> 'Kevin Bacon'