CREATE DATABASE IF NOT EXISTS peluqueria
    CHARACTER SET utf8mb4
    COLLATE utf8mb4_spanish_ci;

USE peluqueria;

CREATE TABLE IF NOT EXISTS clientes (
    id          INT          NOT NULL,
    nombre      VARCHAR(50)  NOT NULL,
    telefono    VARCHAR(20)  NOT NULL,
    email       VARCHAR(100),
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS peluqueras (
    id                  INT          NOT NULL,
    nombre              VARCHAR(50)  NOT NULL,
    especialidad        VARCHAR(50)  NOT NULL,
    telefono            VARCHAR(20)  NOT NULL,
    horas_trabajadas    INT          NOT NULL DEFAULT 0,
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS servicios (
    id          INT             NOT NULL,
    nombre      VARCHAR(50)     NOT NULL,
    descripcion VARCHAR(100),
    duracion    INT             NOT NULL,
    precio      DECIMAL(8, 2)   NOT NULL,
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS reservas (
    id_reserva      INT         NOT NULL,
    id_cliente      INT         NOT NULL,
    id_peluquera    INT         NOT NULL,
    id_servicio     INT         NOT NULL,
    fecha           DATE        NOT NULL,
    hora            TIME        NOT NULL,
    PRIMARY KEY (id_reserva),
    CONSTRAINT fk_reserva_cliente
        FOREIGN KEY (id_cliente)   REFERENCES clientes (id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    CONSTRAINT fk_reserva_peluquera
        FOREIGN KEY (id_peluquera) REFERENCES peluqueras (id)
        ON DELETE RESTRICT ON UPDATE CASCADE,
    CONSTRAINT fk_reserva_servicio
        FOREIGN KEY (id_servicio)  REFERENCES servicios (id)
        ON DELETE RESTRICT ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS config (
    clave   VARCHAR(50)  NOT NULL,
    valor   VARCHAR(100) NOT NULL,
    PRIMARY KEY (clave)
);

CREATE TABLE IF NOT EXISTS logs (
    id          INT             NOT NULL AUTO_INCREMENT,
    mensaje     VARCHAR(500)    NOT NULL,
    fecha_hora  DATETIME        NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id)
);

INSERT INTO config (clave, valor) VALUES
    ('horario_apertura',      '09:00'),
    ('horario_cierre',        '20:00'),
    ('duracion_por_defecto',  '60'),
    ('antelacion_maxima',     '7'),
    ('admin_user',            'admin'),
    ('admin_pass',            '1234');

INSERT INTO clientes (id, nombre, telefono, email) VALUES
    (1, 'Ana García',    '600111222', 'ana@email.com'),
    (2, 'María López',   '600333444', 'maria@email.com'),
    (3, 'Laura Martín',  '600555666', 'laura@email.com');

INSERT INTO peluqueras (id, nombre, especialidad, telefono, horas_trabajadas) VALUES
    (1, 'Carmen Ruiz',   'Coloración',  '611100200', 0),
    (2, 'Sofía Torres',  'Corte',       '611300400', 0),
    (3, 'Paula Gómez',   'Peinado',     '611500600', 0);

INSERT INTO servicios (id, nombre, descripcion, duracion, precio) VALUES
    (1, 'Corte',         'Corte de pelo con lavado',         45,  25.00),
    (2, 'Tinte',         'Coloración completa',              90,  55.00),
    (3, 'Mechas',        'Mechas o balayage',                120, 70.00),
    (4, 'Peinado',       'Peinado para evento especial',     60,  40.00);

INSERT INTO reservas (id_reserva, id_cliente, id_peluquera, id_servicio, fecha, hora) VALUES
    (1, 1, 1, 1, '2026-04-12', '10:00'),
    (2, 2, 1, 2, '2026-04-13', '11:30'),
    (3, 3, 2, 1, '2026-04-14', '09:00'),
    (5, 2, 3, 4, '2026-04-15', '12:00');

CREATE OR REPLACE VIEW v_reservas_detalle AS
    SELECT
        r.id_reserva,
        c.nombre        AS cliente,
        p.nombre        AS peluquera,
        s.nombre        AS servicio,
        s.precio,
        r.fecha,
        r.hora
    FROM reservas r
    JOIN clientes   c ON c.id = r.id_cliente
    JOIN peluqueras p ON p.id = r.id_peluquera
    JOIN servicios  s ON s.id = r.id_servicio;

CREATE OR REPLACE VIEW v_reservas_por_cliente AS
    SELECT
        c.id,
        c.nombre        AS cliente,
        COUNT(r.id_reserva) AS total_reservas
    FROM clientes c
    LEFT JOIN reservas r ON r.id_cliente = c.id
    GROUP BY c.id, c.nombre;

CREATE OR REPLACE VIEW v_servicios_mas_demandados AS
    SELECT
        s.id,
        s.nombre        AS servicio,
        COUNT(r.id_reserva) AS total_usos
    FROM servicios s
    LEFT JOIN reservas r ON r.id_servicio = s.id
    GROUP BY s.id, s.nombre
    ORDER BY total_usos DESC;

CREATE OR REPLACE VIEW v_facturacion_por_peluquera AS
    SELECT
        p.id,
        p.nombre            AS peluquera,
        COALESCE(SUM(s.precio), 0) AS facturacion_total
    FROM peluqueras p
    LEFT JOIN reservas  r ON r.id_peluquera = p.id
    LEFT JOIN servicios s ON s.id = r.id_servicio
    GROUP BY p.id, p.nombre;

CREATE OR REPLACE VIEW v_facturacion_total AS
    SELECT SUM(s.precio) AS total_estimado
    FROM reservas r
    JOIN servicios s ON s.id = r.id_servicio;