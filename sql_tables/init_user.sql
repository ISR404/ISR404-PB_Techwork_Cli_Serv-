CREATE TABLE Users(
    id SERIAL PRIMARY KEY,
    username varchar(40) not null,
    passwd varchar(40) not null,
    tokens int not null
);