 1. Жанры (3) 
INSERT INTO Song_Genre (name) VALUES
('Rock'),
('Pop'),
('Electronic');

 2. Исполнители (4) 
INSERT INTO Artist (first_name) VALUES
('The Beatles'),
('Madonna'),
('Daft Punk'),
('Queen');

 3. Альбомы (3) 
INSERT INTO Album (name, year) VALUES
('Abbey Road', 1969),
('Like a Virgin', 1984),
('Discovery', 2001);

 4. Треки (6) – все принадлежат какому-то альбому 
INSERT INTO Trek (id_Album, name, duration) VALUES
(1, 'Come Together', '0420'),
(1, 'Something', '0303'),
(2, 'Like a Virgin', '0338'),
(2, 'Material Girl', '0400'),
(3, 'One More Time', '0520'),
(3, 'Harder, Better, Faster, Stronger', '0345');

 5. Сборники (4) 
INSERT INTO Collection (name, year) VALUES
('Best of 60-70s', 1995),
('Pop Hits 80s', 1990),
('Electronic Anthems', 2010),
('All-Time Rock', 2020);

-- 4. Связываем исполнителей с жанрами
INSERT INTO Song_Genre_Artist (id_Gener, id_Artist) VALUES
(1, 1), 
(2, 2), 
(3, 3), 
(1, 4); 

INSERT INTO album_artist  (id_album, id_Artist) VALUES
(1, 1), 
(2, 2), 
(3, 3), 
(4, 1); 

 4. Треки (6) – все принадлежат какому-то альбому 
INSERT INTO Trek (id_Album, name, duration) VALUES
(1, 'Come Together', '0420'),
(1, 'Something', '0303'),
(2, 'Like a Virgin', '0338'),
(2, 'Material Girl', '0400'),
(3, 'One More Time', '0520'),
(3, 'Harder, Better, Faster, Stronger', '0345');

 5. Сборники (4) 
INSERT INTO Collection (name, year) VALUES
('Best of 60-70s', 2025),
('Pop Hits 80s', 2024),
('Electronic Anthems', 2020),
('All-Time Rock', 2015);

 8. Связи Трек ↔ Сборник (по 3-4 трека в сборник) 
INSERT INTO Trek_Collection (id_Trek, id_Collection) VALUES
(1, 1),
(2, 1),
(3, 2),
(4, 2),
(5, 3),
(6, 3),
(1, 4),
(2, 4),
(6, 4);   -- можно подключить любой трек, просто для примера