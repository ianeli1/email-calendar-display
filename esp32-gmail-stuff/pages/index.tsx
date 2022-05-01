import { AppShell, Button, Loader } from "@mantine/core";
import type { NextPage } from "next";
import { signIn, signOut, useSession } from "next-auth/react";
import Head from "next/head";
import Image from "next/image";
import styles from "../styles/Home.module.css";

const Home: NextPage = () => {
  const { status, data } = useSession();
  return (
    <AppShell>
      <h1>{`Hello there${data?.user?.name ? `, ${data.user?.name}` : ""}`}</h1>
      {status == "authenticated" ? (
        <Button onClick={() => signOut()}>Log out</Button>
      ) : status === "unauthenticated" ? (
        <Button onClick={() => signIn("google")}>Log in</Button>
      ) : (
        <Loader />
      )}
    </AppShell>
  );
};

export default Home;
