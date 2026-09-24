function sleep(ms) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}

async function run() {
  const QUERY_STRING = process.env.QUERY_STRING
  console.log(QUERY_STRING ? "" : "Please update QUERY_STRING")
  splits = QUERY_STRING.split("&")
  qs = {}
  for (s in splits) {
    let tmp = splits[s].split("=")
    qs[tmp[0]] = tmp[1]
  }
  if (qs.time)
    await sleep(qs.time * 1000)
    console.log("Successfuly waited for " + qs.time + " seconds")
}

run()
