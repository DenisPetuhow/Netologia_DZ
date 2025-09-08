SELECT name, duration FROM   Trek
WHERE duration = (SELECT MAX(duration) FROM Trek);

SELECT name FROM   Trek
WHERE  duration >= TIME '03:30:00'
ORDER BY duration;

SELECT name FROM   Collection
WHERE  year BETWEEN 2018 AND 2020;

SELECT first_name FROM   Artist
WHERE  first_name !~ '\s';   -- нет пробельных символов

INSERT INTO Trek (id_album, name, duration) VALUES
(1, 'My Favourite Song', '03:15'),
(2, 'Это мой выбор',     '04:00');

SELECT name FROM   Trek
WHERE  LOWER(name) LIKE '%my%'
   OR  LOWER(name) LIKE '%мой%';


SELECT g.name, COUNT(sga.id_artist) AS artist_cnt
FROM   song_genre g
JOIN   song_genre_artist sga ON sga.id_gener = g.id
GROUP  BY g.name
ORDER  BY artist_cnt DESC;

-- 2. Число треков, вошедших в альбомы 2023-2025 годов
SELECT COUNT(*) AS tracks_2019_20
FROM   trek  t
JOIN   album a ON a.id = t.id_album
WHERE  a.year BETWEEN 2023 AND 2025;

-- 4. Исполнители, не выпустившие альбомов в 2020 году
SELECT first_name
FROM   artist
EXCEPT
SELECT ar.first_name
FROM   artist      ar
JOIN   album_artist aa ON aa.id_artist = ar.id
JOIN   album       al  ON al.id       = aa.id_album
WHERE  al.year = 2020;

-- 5. Сборники, содержащие треки исполнителя «Queen»
SELECT DISTINCT c.name
FROM   collection    c
JOIN   trek_collection tc ON tc.id_collection = c.id
JOIN   trek          t   ON t.id = tc.id_trek
JOIN   album         a   ON a.id = t.id_album
JOIN   album_artist  aa  ON aa.id_album = a.id
JOIN   artist        ar  ON ar.id = aa.id_artist
WHERE  ar.first_name = 'Queen';