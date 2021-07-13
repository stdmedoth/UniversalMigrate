-- migrate made with migrate script  

-- Do not edit this file unless you know what you are doing.
-- You should create a new migrate with ./migrate new

CREATE TABLE IF NOT EXISTS versao( code INT PRIMARY KEY AUTO_INCREMENT, versao VARCHAR(30) NOT NULL, lancamento DATETIME NOT NULL);

CREATE TABLE IF NOT EXISTS migrate( code INT PRIMARY KEY AUTO_INCREMENT, descricao VARCHAR(300) NOT NULL, arquivo VARCHAR(150) NOT NULL, versao VARCHAR(30) NOT NULL,lancamento DATETIME NOT NULL); 

INSERT IGNORE INTO versao VALUES(1, '1.0', '2021-05-28');