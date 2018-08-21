const StreamlabsSocketClient = require('streamlabs-socket-client');
 
const client = new StreamlabsSocketClient({
  token: 'eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ0b2tlbiI6IjJCOEY0QjFCREZENjI1RDlDQzg4IiwicmVhZF9vbmx5Ijp0cnVlLCJwcmV2ZW50X21hc3RlciI6dHJ1ZSwieW91dHViZV9pZCI6IlVDMXM3UGhna181a085U1oxdWxxQ0ZBQSJ9.zX3-YGVy7Z1iYXaYZwVSf4xwNNaIGiPMV-LneoeN4CU',
  emitTests: true // true if you want alerts triggered by the test buttons on the streamlabs dashboard to be emitted. default false.
});
 
client.on('donation', (data) => {
  console.log(data);
});
 
client.on('follow', (data) => {
  console.log(data);
});

client.connect();