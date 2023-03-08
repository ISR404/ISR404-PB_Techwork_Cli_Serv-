CREATE TABLE Users(
    id SERIAL PRIMARY KEY,
    username varchar(40) not null,
    passwd varchar(40) not null,
    tokens int not null
);

INSERT INTO Users(username, passwd, tokens) VALUES('test_user', 'test_password', 10);