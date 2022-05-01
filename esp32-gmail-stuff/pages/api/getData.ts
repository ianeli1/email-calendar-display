import { NextApiRequest, NextApiResponse } from "next";
import { gmail, auth, gmail_v1 } from "@googleapis/gmail";
import clientPromise from "../../lib/mongodb";

interface Account {
  providerAccountId: string;
  access_token: string;
  expires_at: number;
  refresh_token: string;
  scope: string;
  token_type: "Bearer";
  id_token: string;
  userId: string;
}

const options: ConstructorParameters<typeof Intl.DateTimeFormat>[1] = {
  timeZone: "America/Costa_Rica",
  month: "numeric",
  day: "numeric",
  hour: "numeric",
  minute: "numeric",
  hour12: false,
};

function findHeader(
  payload: gmail_v1.Schema$MessagePart | undefined,
  header: string
) {
  return payload?.headers
    ?.find(({ name }) => name === header)
    ?.value?.slice(0, 18);
}

function getTime() {
  const formatter = new Intl.DateTimeFormat([], options);
  const date = formatter
    .format(new Date())
    .split(",")
    .map((x) => x.trim());
  return {
    date: date[0],
    time: date[1],
  };
}

export default async function handler(
  req: NextApiRequest,
  res: NextApiResponse
) {
  const client = await clientPromise;
  const oauth = new auth.OAuth2({
    clientId: process.env.GOOGLE_CLIENT_ID!,
    clientSecret: process.env.GOOGLE_CLIENT_SECRET!,
  });
  const account = (
    await client
      .db("esp32")
      .collection<Account>("accounts")
      .find()
      .sort({
        _id: "descending",
      })
      .limit(1)
      .toArray()
  )[0];

  oauth.setCredentials({
    ...account,
    expiry_date: account.expires_at,
  });
  const gmail_me = gmail({
    version: "v1",
    auth: oauth,
  });

  const { data } = await gmail_me.users.labels.get({
    userId: account.providerAccountId,
    //q: "label:unread -category:promotions -category:social",
    id: "INBOX",
  });
  const {
    data: { messages },
  } = await gmail_me.users.messages.list({
    userId: account.providerAccountId,
    q: "label:unread",
    maxResults: 1,
  });
  let subject = "";
  let from = "";
  if (messages?.length && messages[0].id) {
    const {
      data: { payload },
    } = await gmail_me.users.messages.get({
      userId: account.providerAccountId,
      id: messages[0].id!,
    });
    subject = findHeader(payload, "Subject") ?? "";
    from = findHeader(payload, "From")?.split("<")[0].trim().slice(0, 18) ?? "";
  }

  const date = getTime();

  res.status(200).json({
    unread: data.threadsUnread ?? data.messagesUnread ?? 0,
    subject,
    from,
    date: date.date,
    time: date.time,
  });
}
