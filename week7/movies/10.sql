SELECT DISTINCT p.name
FROM directors d
JOIN movies m
    ON m.id = d.movie_id
JOIN people p
    ON p.id = d.person_id
JOIN ratings r
    ON r.movie_id = m.id
WHERE
    r.rating >= 9.0