CREATE TABLE IF NOT EXISTS Song_Genre (
id SERIAL PRIMARY KEY,
name VARCHAR(40) NOT NULL
);

CREATE TABLE IF NOT EXISTS Artist (
id  SERIAL PRIMARY KEY,
first_name VARCHAR(40) NOT NULL
);

CREATE TABLE IF NOT EXISTS Song_Genre_Artist (
id_Gener INTEGER REFERENCES Song_Genre(id),
id_Artist INTEGER REFERENCES Artist(id),
CONSTRAIN pk PRIMARY KEY (id_Gener, id_Artist)
);


CREATE TABLE IF NOT EXISTS Album(
id  SERIAL PRIMARY KEY,
name VARCHAR(40) NOT NULL,
year YEAR
);

CREATE TABLE IF NOT EXISTS Album_Artist (
id_Album INTEGER REFERENCES Album(id),
id_Artist INTEGER REFERENCES Artist(id),
CONSTRAIN pk PRIMARY KEY (id_Album, id_Artist)
);

CREATE TABLE IF NOT EXISTS Trek(
id  SERIAL PRIMARY KEY,
id_Album INTEGER NOT NULL REFERENCES Album(id),
name VARCHAR(40) NOT NULL,
duration TIME
);

CREATE TABLE IF NOT EXISTS Collection(
id  SERIAL PRIMARY KEY,
name VARCHAR(40) NOT NULL,
year YEAR
);

CREATE TABLE IF NOT EXISTS Trek_Collection (
id_Trek INTEGER REFERENCES Trek(id),
id_Collection INTEGER REFERENCES Collection(id),
CONSTRAIN pk PRIMARY KEY (id_Trek, id_Collection)
);

--REFERENCES ограничение, используется в 1-1 Говорит что существует, если есть в ой таблице откуда ссылается данные
--CONSTRAIN ограничение, Позволяет реализовать смежную связь , если ограничение на primeri key  у двух смежных столбцов, то не получится создать повторающиеся строки