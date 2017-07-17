CREATE TABLE `screenText` (
  `idscreenText` int(11) NOT NULL AUTO_INCREMENT,
  `ScreenLayoutId` varchar(10) DEFAULT NULL,
  `ScreenLocId` varchar(10) DEFAULT NULL,  
  `labelText` varchar(80) DEFAULT NULL,
  `labelRow` int(11) DEFAULT NULL,
  `labelCol` int(11) DEFAULT NULL,  
  PRIMARY KEY (`idscreenText`)
) ENGINE=InnoDB AUTO_INCREMENT=99 DEFAULT CHARSET=latin1;
