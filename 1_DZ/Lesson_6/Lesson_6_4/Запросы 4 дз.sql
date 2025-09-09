WITH album_multigenre AS (
    SELECT aa.id_album
    FROM album_artist aa
    JOIN song_genre_artist sga ON sga.id_artist = aa.id_artist
    GROUP BY aa.id_album
    HAVING COUNT(DISTINCT sga.id_gener) > 1
)
SELECT a.name
FROM   album a
JOIN   album_multigenre am ON am.id_album = a.id;

WITH shortest AS (
    SELECT MIN(duration) AS min_dur
    FROM   trek
)
SELECT DISTINCT ar.first_name
FROM   shortest      sh
JOIN   trek          t  ON t.duration = sh.min_dur
JOIN   album         a  ON a.id = t.id_album
JOIN   album_artist  aa ON aa.id_album = a.id
JOIN   artist        ar ON ar.id = aa.id_artist;

SELECT t.name
FROM   trek t
WHERE  NOT EXISTS (
        SELECT 1
        FROM trek_collection tc
        WHERE tc.id_trek = t.id
      );

WITH tracks_per_album AS (
    SELECT id_album,
           COUNT(*) AS cnt
    FROM   trek
    GROUP  BY id_album
),
min_tracks AS (
    SELECT MIN(cnt) AS min_cnt
    FROM   tracks_per_album
)
SELECT a.name
FROM   tracks_per_album tpa
JOIN   min_tracks       mt ON mt.min_cnt = tpa.cnt
JOIN   album            a  ON a.id = tpa.id_album;