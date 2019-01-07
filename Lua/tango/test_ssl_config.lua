return {
  client = {
    mode = 'client',
    protocol = 'tlsv1',
    verify = 'none',
    options = 'all',	
    ciphers = 'ALL:!ADH:@STRENGTH'    
  },
  server = {
    mode = 'server',
    protocol = 'tlsv1',
    options = {'all'},
    verify = {'peer'},    
    options = {'all'},
    ciphers = 'ALL:!ADH:@STRENGTH',	
    certificate = './test_server.cert',
    key = './test_server.key'
  }
}
